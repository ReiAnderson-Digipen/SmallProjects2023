#[allow(non_snake_case)]

mod MyOption{
    #[derive(Debug)]

    pub enum MyOption<T>{
        None,
        Some(T)
    }
    impl<T> MyOption<T>{
        pub fn is_none(&self) -> bool{
            match self {
                MyOption::Some(_) => false,
                MyOption::None => true
            }
        }
        pub fn unwrap(self) -> T{
            match self {
                MyOption::None => panic!(),
                MyOption::Some(value) => value            
            }
            
        }
    
    }
    

}







fn might_fail(num : i32) -> MyOption::MyOption<i32>{
    if num < 5{
        MyOption::MyOption::Some(num)
    }
    else{
        MyOption::MyOption::None
    }
}

fn main() {
    for i in 0..10{
        let v = might_fail(i);
        if !v.is_none(){
            println!("{0}", v.unwrap());
        }
        else{
            println!("FAILED");
        }
        
    }
}
