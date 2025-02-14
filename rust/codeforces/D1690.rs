use std::io;

fn main() {
    let mut input:String = String::default();
    let t:i16;
    let mut n:i32;
    let mut k:i32;
    let mut min:i32;
    let mut count:i32;

    io::stdin().read_line(&mut input).expect("No input found");
    t = input.trim().parse::<i16>().expect("No first int");

    for _bigger in 0..t {
        input = String::default();
        io::stdin().read_line(&mut input).expect("No input found");
        n = input.trim().split(' ').nth(0).unwrap().parse::<i32>().expect("No int");
        k = input.trim().split(' ').nth(1).unwrap().parse::<i32>().expect("No int");
        input = String::default();
        io::stdin().read_line(&mut input).expect("No input");

        min = n;
        for i in 0..n - k + 1{
            count = 0;
            for j in i..i + k {
                if input.chars().nth(j as usize).unwrap() == 'W'{
                    count += 1;
                }
            }
            if min > count {
                min = count;
            }
        }
        println!("{}", min);

    }
}
