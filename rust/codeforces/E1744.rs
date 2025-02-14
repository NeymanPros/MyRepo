use std::io;

fn main() {
    let mut input:String = String::default();
    io::stdin().read_line(&mut input).expect("No input");
    let t:i8;
    t = input.trim().parse::<i8>().expect("No first int");

    let mut a:i32;
    let mut b:i32;
    let mut c:i32;
    let mut d:i32;

    let mut del:i64;
    for _i in 0..t {
        input = String::default();
        io::stdin().read_line(&mut input).expect("Worst");
        a = input.trim().split(' ').nth(0).unwrap().parse::<i32>().expect("No int");
        b = input.trim().split(' ').nth(1).unwrap().parse::<i32>().expect("No int");
        c = input.trim().split(' ').nth(2).unwrap().parse::<i32>().expect("No int");
        d = input.trim().split(' ').nth(3).unwrap().parse::<i32>().expect("No int");

        del = a as i64 * b as i64;
    }
}
