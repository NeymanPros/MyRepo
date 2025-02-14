use std::io;

fn main() {
//    let mut a:Vec<&str>;
    let t:i32;
    let mut input:String = String::default();

    io::stdin().read_line(&mut input).expect("error: no input");
    t = input.trim().parse::<i32>().expect("no int");

    for _i in 0..t {
        input = String::default();
        io::stdin().read_line(&mut input).expect("no input");
        let mut a:Vec<i16> = vec![];
        let b = input.trim().split(" ");
        b.for_each(|x:&str| {  a.push(x.parse::<i16>().expect("error")) });

        for i in 0..5 {
            if a[0] <= a[1] && a[0] <= a[2] {
                a[0] += 1;
            } else if a[1] <= a[0] && a[1] <= a[2] {
                a[1] += 1;
            } else { a[2] += 1; }
        }
        println!("{}", a[0]*a[1]*a[2]);
    }
}
