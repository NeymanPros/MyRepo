use std::io;

fn main() {
    let n:i16;
    let mut y:i8;
    let mut read:String = String::new();
    io::stdin().read_line(&mut read).expect("error: no input");
    n = read.trim().parse::<i16>().expect("error: this isn't an integer");
    for _i in 0..n {
        read = String::new();
        io::stdin().read_line(&mut read).expect("error: no input");
        read = read.replacen("\n", " ", (n - 1) as usize);
        let mut out = read.trim().split(' ');
        out.nth(0);
        y = out.nth(0).unwrap().parse::<i8>().unwrap();
        if y >= -1 {
            println!("YES");
        }else{
            println!("NO");
        }
    }
}
