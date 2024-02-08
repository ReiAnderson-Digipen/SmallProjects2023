#include "ObjectAllocator.h"
#include <cstring>
#define UNUSED(a) reinterpret_cast<const void*>(a)

ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config)
{
  config_ = config;
  stats_.ObjectSize_ = ObjectSize;
  stats_.FreeObjects_ = 0;
  stats_.PagesInUse_ = 0;
  block_size = static_cast<unsigned>(ObjectSize) + (2*config_.PadBytes_) + static_cast<unsigned>(config_.HBlockInfo_.size_);
  if (config.Alignment_ == 0 || config.Alignment_ == 1)
  {
    config_.LeftAlignSize_ = config_.InterAlignSize_ = 0;
  }
  else
  {
    config_.LeftAlignSize_ = config_.Alignment_ - (static_cast<unsigned>(sizeof(GenericObject)) % config_.Alignment_);
    config_.InterAlignSize_ = (config_.Alignment_ - (block_size % config_.Alignment_)%config_.Alignment_);
  }


  
  debug_ = config.DebugOn_;
  stats_.PageSize_ = sizeof(GenericObject)+block_size * config_.ObjectsPerPage_;

  page_count = 0;
  PageList_ = nullptr;
  FreeList_ = nullptr;
  if(debug_) AllocNewPage();
  header_block_offset = (config.PadBytes_+static_cast<unsigned>(config.HBlockInfo_.size_));
}

ObjectAllocator::~ObjectAllocator()
{
  if(debug_) while (PageList_)
  {
    auto next = PageList_->Next;
    auto casted = reinterpret_cast<char*>(PageList_);
    delete[] casted;
    PageList_ = next;
  }
}

void* ObjectAllocator::Allocate(const char* label)
{
  if (debug_)
  {
    if (FreeList_ == nullptr)
    {
      AllocNewPage();
    }
    if (FreeList_)
    {

      char* obj = reinterpret_cast<char*>(FreeList_);
      char* header = obj - header_block_offset;
      

      //memset(obj-config_.PadBytes_, PAD_PATTERN, config_.PadBytes_);
      //obj += config_.HBlockInfo_.size_;
      FreeList_ = FreeList_->Next;

      //obj += config_.PadBytes_;

      memset(obj, ALLOCATED_PATTERN, stats_.ObjectSize_);





      stats_.ObjectsInUse_++;
      MemBlockInfo* external = nullptr;
      if (config_.HBlockInfo_.type_ == OAConfig::hbExternal)
      {
        external = new MemBlockInfo();
        if (label)
        {
          external->label = new char[strlen(label) + 1];
          strcpy(external->label, label);
        }
        external->alloc_num = stats_.ObjectsInUse_;
        external->in_use = 1;
      }
      WriteHeader(header, external);
      stats_.Allocations_++;
      stats_.FreeObjects_--;

      //memset(obj+stats_.ObjectSize_, PAD_PATTERN, config_.PadBytes_);
      if (stats_.ObjectsInUse_ > stats_.MostObjects_) stats_.MostObjects_ = stats_.ObjectsInUse_;
      return reinterpret_cast<void*>(obj);

    }
    throw OAException(OAException::OA_EXCEPTION::E_NO_MEMORY, "You have run out of memory");
  }
  else
  {
    stats_.ObjectsInUse_++;
    stats_.Allocations_++;
    if (stats_.ObjectsInUse_ > stats_.MostObjects_) stats_.MostObjects_ = stats_.ObjectsInUse_;
    return reinterpret_cast<void*>(new char[stats_.ObjectSize_]);
  }
}

void ObjectAllocator::Free(void* Object)
{
  if (debug_)
  {
    //Object = reinterpret_cast<void*>(reinterpret_cast<char*>(Object));
    char* header = reinterpret_cast<char*>(Object) - header_block_offset;
    if (!IsAlligned(reinterpret_cast<char*>(Object) - config_.PadBytes_ - config_.HBlockInfo_.size_))
    {
      throw OAException(OAException::OA_EXCEPTION::E_BAD_BOUNDARY, "This object is not alligned");
    }
    if (config_.PadBytes_ > 0 && CheckCorrupt(reinterpret_cast<char*>(Object)))
    {
      throw OAException(OAException::OA_EXCEPTION::E_CORRUPTED_BLOCK, "THis is corrupted!");
    }
    if (InFreeList(Object) || Object == nullptr)
    {
      throw OAException(OAException::OA_EXCEPTION::E_MULTIPLE_FREE, "You have freed this already");
    }

    else if (FreeList_ == nullptr)
    {

      FreeList_ = reinterpret_cast<GenericObject*>(Object);
      memset(Object, FREED_PATTERN, stats_.ObjectSize_);
      FreeList_->Next = nullptr;
      stats_.FreeObjects_++;
      stats_.Deallocations_++;
      stats_.ObjectsInUse_--;
      ClearHeader(header);
      return;
    }
    auto current_page = PageList_;
    auto current_free = FreeList_;
    GenericObject* last_free = nullptr;


    while (current_page && !InPage(current_page, Object))
    {
      current_page = current_page->Next;
    }
    while (current_page && current_free && !InPage(current_page, current_free))
    {
      last_free = current_free;
      current_free = current_free->Next;
    }
    if (current_free == nullptr && current_page)
    {

      auto freed_block = reinterpret_cast<GenericObject*>(Object);
      memset(Object, FREED_PATTERN, stats_.ObjectSize_);
      freed_block->Next = last_free->Next;
      last_free->Next = freed_block;
      stats_.Deallocations_++;
      stats_.ObjectsInUse_--;
      stats_.FreeObjects_++;
      ClearHeader(header);

      return;
    }
    else if (current_page == nullptr)
    {
      throw OAException(OAException::OA_EXCEPTION::E_BAD_BOUNDARY, "Object not in allocator");
    }
    else
    {
      auto freed_block = reinterpret_cast<GenericObject*>(Object);
      memset(Object, FREED_PATTERN, stats_.ObjectSize_);
      if (last_free == nullptr) last_free = current_free, current_free = current_free->Next;
      last_free->Next = freed_block;
      freed_block->Next = current_free;
      stats_.Deallocations_++;
      stats_.ObjectsInUse_--;
      stats_.FreeObjects_++;
      ClearHeader(header);
    }
  }
  else
  {
    delete reinterpret_cast<char*>(Object);
    stats_.Deallocations_++;
    stats_.ObjectsInUse_--;
  }
}

unsigned ObjectAllocator::DumpMemoryInUse(DUMPCALLBACK fn) const
{
  auto current = PageList_;
  unsigned block_count = 0;
  while (current)
  {
    char* data = reinterpret_cast<char*>(current + 1) + config_.LeftAlignSize_;
    for (unsigned i = 0; i < config_.ObjectsPerPage_; i++)
    {
      if (!InFreeList(data+header_block_offset))
      {
        block_count++;
        fn(data+header_block_offset, stats_.ObjectSize_);
      }
      data += block_size;
    }
    current = current->Next;
  }
  return block_count;
}

unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
  auto current_page = PageList_;
  unsigned corruptions = 0;
  while (current_page)
  {
    auto block_start = reinterpret_cast<char*>(current_page) + config_.LeftAlignSize_ + sizeof(GenericObject);
    for (unsigned i = 0; i < config_.ObjectsPerPage_; i++)
    {
      if (CheckCorrupt(block_start+header_block_offset))
      {
        fn(block_start, stats_.ObjectSize_);
        corruptions++;
      }
      block_start += block_size;
    }
    current_page = current_page->Next;
  }
  
  return corruptions;
}

unsigned ObjectAllocator::FreeEmptyPages()
{
  return 0;
}

bool ObjectAllocator::ImplementedExtraCredit()
{
  return true;
}

void ObjectAllocator::AllocNewPage()
{
  if (page_count < config_.MaxPages_ || config_.MaxPages_ == 0)
  {
    char* data = new char[stats_.PageSize_];
    
    GenericObject* new_page = reinterpret_cast<GenericObject*>(data);
    new_page->Next = PageList_;

    char* current_padding = data + sizeof(GenericObject) + config_.LeftAlignSize_+config_.HBlockInfo_.size_;
    GenericObject* current_free = reinterpret_cast<GenericObject*>(data + sizeof(GenericObject) + config_.LeftAlignSize_ + config_.PadBytes_+config_.HBlockInfo_.size_);
    memset(current_free, UNALLOCATED_PATTERN, stats_.ObjectSize_);
    current_free->Next = FreeList_;
    memset(current_padding, PAD_PATTERN, config_.PadBytes_);
    memset(current_padding + block_size - config_.PadBytes_-config_.HBlockInfo_.size_, PAD_PATTERN, config_.PadBytes_);
    current_padding += block_size;

    for (unsigned i = 0; i < config_.ObjectsPerPage_ - 1; i++)
    {
      memset(current_padding, PAD_PATTERN, config_.PadBytes_);
      memset(current_padding + block_size - config_.PadBytes_- config_.HBlockInfo_.size_, PAD_PATTERN, config_.PadBytes_);
      current_padding += block_size;
      auto old = current_free;
      current_free = reinterpret_cast<GenericObject*>(reinterpret_cast<char*>(current_free) + block_size);
      memset(current_free, UNALLOCATED_PATTERN, stats_.ObjectSize_);
      current_free->Next = old;
    }

    stats_.PagesInUse_++;
    stats_.FreeObjects_ += config_.ObjectsPerPage_;
    FreeList_ = current_free;
    PageList_ = new_page;
    page_count++;
  }
  else
  {
    throw OAException(OAException::OA_EXCEPTION::E_NO_PAGES, "You are out of pages");
  }
}

bool ObjectAllocator::InFreeList(void* block) const
{
  auto current = FreeList_;
  while (current)
  {
    if (reinterpret_cast<void*>(current) == block)
    {
      return true;
    }
    current = current->Next;
  }
  return false;
}



bool ObjectAllocator::InPage(const GenericObject* page, void* block) const
{
  const char* data = reinterpret_cast<const char*>(page);
  const char* end = data + stats_.PageSize_;
  return block > data && block < end;
}

bool ObjectAllocator::IsAlligned(char* addr) const
{
  auto current = PageList_;
  while (current)
  {
    if (InPage(current, addr))
    {
      char* first_block = reinterpret_cast<char*>(current) + sizeof(GenericObject) + config_.LeftAlignSize_;
      if ((addr - first_block) % block_size == 0)
      {
        return true;
      }
      return false;
    }
    current = current->Next;

  }
  return false;
}

void ObjectAllocator::WriteHeader(char* header, MemBlockInfo* external)
{
  if (config_.HBlockInfo_.type_ == OAConfig::hbBasic)
  {
    unsigned* alloc_count = reinterpret_cast<unsigned*>(header);
    *alloc_count = stats_.ObjectsInUse_;
    char* flag = reinterpret_cast<char*>(header + sizeof(unsigned));
    *flag = 0x1;
  }
  
  else if (config_.HBlockInfo_.type_ == OAConfig::hbExtended)
  {
    char* after_user = header + config_.HBlockInfo_.additional_;
    unsigned short* reuse = reinterpret_cast<unsigned short*>(after_user);
    unsigned* obj_num = reinterpret_cast<unsigned*>(after_user + sizeof(unsigned short));
    unsigned char* in_use = reinterpret_cast<unsigned char*>(after_user + sizeof(unsigned short) + sizeof(unsigned));
    (*reuse)++;
    *obj_num = stats_.ObjectsInUse_;
    *in_use = 1;

  }
  else if (config_.HBlockInfo_.type_ == OAConfig::hbExternal)
  {
    MemBlockInfo ** external_ = reinterpret_cast<MemBlockInfo**>(header);
    *external_ = external;
  }


}

void ObjectAllocator::ClearHeader(char* header)
{
  if (config_.HBlockInfo_.type_ == OAConfig::hbBasic)
  {
    memset(header, 0, config_.BASIC_HEADER_SIZE);
  }
  else if (config_.HBlockInfo_.type_ == OAConfig::hbExtended)
  {
    char* after_user = header + config_.HBlockInfo_.additional_;
    //unsigned short* reuse = reinterpret_cast<unsigned short*>(after_user);
    unsigned* obj_num = reinterpret_cast<unsigned*>(after_user + sizeof(unsigned short));
    unsigned char* in_use = reinterpret_cast<unsigned char*>(after_user + sizeof(unsigned short) + sizeof(unsigned));
    *obj_num = 0;
    *in_use = 0;

  }
  else if (config_.HBlockInfo_.type_ == OAConfig::hbExternal)
  {
    MemBlockInfo** external = reinterpret_cast<MemBlockInfo**>(header);
    delete[](*external)->label;
    delete* external;
    memset(header, 0, config_.EXTERNAL_HEADER_SIZE);
  }
}

bool ObjectAllocator::CheckCorrupt(char* object) const
{
  auto expected_padding = new char[config_.PadBytes_];
  memset(expected_padding, PAD_PATTERN, config_.PadBytes_);
  auto left_pad = object - config_.PadBytes_;
  auto right_pad = object + stats_.ObjectSize_;
  auto ret = memcmp(left_pad, expected_padding, config_.PadBytes_) != 0 || memcmp(right_pad, expected_padding, config_.PadBytes_) != 0;
  delete[] expected_padding;
  return ret;
}

void ObjectAllocator::SetDebugState(bool State)
{
  debug_ = State;
  config_.DebugOn_ = State;
}

const void* ObjectAllocator::GetFreeList() const
{
  return reinterpret_cast<void*>(FreeList_);
}

const void* ObjectAllocator::GetPageList() const
{
  return reinterpret_cast<void*>(PageList_);
}

OAConfig ObjectAllocator::GetConfig() const
{
  return config_;
}

OAStats ObjectAllocator::GetStats() const
{
  return stats_;
}
