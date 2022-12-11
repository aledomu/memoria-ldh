use std::io::{stdin, Write, self};

const PORT_PATH: &str = "/dev/ttyACM0";
const BAUD_RATE: u32 = 9_600;

fn main() -> Result<(), io::Error> {
    let mut port = serialport::new(PORT_PATH, BAUD_RATE).open_native()?;

    println!("Port {PORT_PATH} opened");
    println!(
        "Type how many degrees to set the servo to (max. 180 degrees), \
        \'q' to quit"
    );

    stdin()
        .lines()
        .map_while(|l| match l {
            Ok(c) if c == "q" => None,
            a => Some(a),
        })
        .try_fold((), |_, l| match u8::from_str_radix(&l?, 10) {
            Ok(n) if n <= 180 => port.write(&[n]).and(Ok(())),
            _ => Ok(println!("Invalid number, it must be between 0 and 180.")),
        })
}
