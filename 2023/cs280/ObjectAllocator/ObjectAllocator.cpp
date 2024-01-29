#include "ObjectAllocator.h"
#include <cstring>
#define UNUSED(a) reinterpret_cast<const void*>(a)

ObjectAllocator::ObjectAllocator(size_t ObjectSize, const OAConfig& config)
{
  config_ = config;
  stats_.ObjectSize_ = ObjectSize;
  stats_.FreeObjects_ = 0;
  block_size = static_cast<unsigned>(ObjectSize) + config_.PadBytes_ + static_cast<unsigned>(config_.HBlockInfo_.size_);
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
  AllocNewPage();
}

ObjectAllocator::~ObjectAllocator()
{
  while (PageList_)
  {
    auto next = PageList_->Next;
    delete[] reinterpret_cast<char*>(PageList_);
    PageList_ = next;
  }
}

void* ObjectAllocator::Allocate(const char* label)
{
  if (FreeList_ == nullptr)
  {
    AllocNewPage();
  }
  if (FreeList_)
  {
    char* obj = reinterpret_cast<char*>(FreeList_);
    UNUSED(label);
    
    obj += config_.HBlockInfo_.size_;
    FreeList_ = FreeList_->Next;
    memset(obj, ALLOCATED_PATTERN, block_size - config_.HBlockInfo_.size_);
    stats_.ObjectsInUse_++;
    stats_.Allocations_++;
    stats_.FreeObjects_--;
    if (stats_.ObjectsInUse_ > stats_.MostObjects_) stats_.MostObjects_ = stats_.ObjectsInUse_;
    return reinterpret_cast<void*>(obj);

  }
  throw OAException(OAException::OA_EXCEPTION::E_NO_MEMORY, "You have run out of memory");
  
}

void ObjectAllocator::Free(void* Object)
{
  Object = reinterpret_cast<void*>(reinterpret_cast<char*>(Object) - config_.HBlockInfo_.size_);
  if (InFreeList(Object) || Object == nullptr)
  {
    throw OAException(OAException::OA_EXCEPTION::E_MULTIPLE_FREE, "You have freed this already");
  }
  
  else if (FreeList_ == nullptr)
  {
    
    FreeList_ = reinterpret_cast<GenericObject*>(Object);
    memset(Object, FREED_PATTERN, block_size);
    FreeList_->Next = nullptr;
    stats_.FreeObjects_++;
    stats_.Deallocations_++;
    stats_.ObjectsInUse_--;
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
    memset(Object, FREED_PATTERN, block_size);
    freed_block->Next = last_free->Next;
    last_free->Next = freed_block;
    stats_.Deallocations_++;
    stats_.ObjectsInUse_--;
    stats_.FreeObjects_++;
    
    return;
  }
  else if (current_page == nullptr)
  {
    throw OAException(OAException::OA_EXCEPTION::E_BAD_BOUNDARY, "Object not in allocator");
  }
  else
  {
    auto freed_block = reinterpret_cast<GenericObject*>(Object);
    memset(Object, FREED_PATTERN, block_size);
    if (last_free == nullptr) last_free = current_free, current_free = current_free->Next;
    last_free->Next = freed_block;
    freed_block->Next = current_free;
    stats_.Deallocations_++;
    stats_.ObjectsInUse_--;
    stats_.FreeObjects_++;
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
      if (!InFreeList(data))
      {
        block_count++;
        fn(data, block_size);
      }
      data += block_size;
    }

  }
  return block_count;
}

unsigned ObjectAllocator::ValidatePages(VALIDATECALLBACK fn) const
{
  fn(nullptr, 0);
  return 0;
}

unsigned ObjectAllocator::FreeEmptyPages()
{
  return 0;
}

bool ObjectAllocator::ImplementedExtraCredit()
{
  return false;
}

void ObjectAllocator::AllocNewPage()
{
  if (page_count < config_.MaxPages_ || page_count == 0)
  {
    
    char* data = new char[stats_.PageSize_];
    memset(data+sizeof(GenericObject), UNALLOCATED_PATTERN, stats_.PageSize_-sizeof(GenericObject));
    GenericObject* new_page = reinterpret_cast<GenericObject*>(data);
    new_page->Next = PageList_;
    PageList_ = new_page->Next;

    // if (PageList_ == nullptr)
    // {
    //   PageList_ = reinterpret_cast<GenericObject*>(data);
    //   last_page = PageList_;
    // }
    // else
    // {
    //   last_page = PageList_;
    //   while (last_page->Next)
    //   {
    //     last_page = last_page->Next;
    //   }
    //   last_page->Next = reinterpret_cast<GenericObject*>(data);
    // }

    char* current_free = (data + config_.LeftAlignSize_ + sizeof(GenericObject));
    reinterpret_cast<GenericObject*>(current_free)->Next = nullptr;
    if (FreeList_)
      FreeList_->Next = reinterpret_cast<GenericObject*>(current_free);
    for (unsigned i = 0; i < config_.ObjectsPerPage_ - 1; i++)
    {
      auto old = current_free;
      current_free = (current_free + block_size + config_.InterAlignSize_);
      reinterpret_cast<GenericObject*>(current_free)->Next = reinterpret_cast<GenericObject*>(old);
    }
    if(!FreeList_)
      FreeList_ = reinterpret_cast<GenericObject*>(current_free);
    stats_.PagesInUse_++;
    page_count++;
    stats_.FreeObjects_ += config_.ObjectsPerPage_;

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
