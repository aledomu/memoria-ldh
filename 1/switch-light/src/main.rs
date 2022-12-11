use console::Term;
use std::{error::Error, io::Write};

const PORT_PATH: &str = "/dev/ttyACM0";
const BAUD_RATE: u32 = 9_600;

fn main() -> Result<(), Box<dyn Error>> {
    let mut port = serialport::new(PORT_PATH, BAUD_RATE).open_native()?;

    println!("Port {PORT_PATH} opened");
    println!("Press 'h' to switch on LED, 'l' to switch off LED, 'q' to quit");

    let term = Term::stdout();
    loop {
        match term.read_char()? {
            'h' => {
                write!(port, "h")?;
                println!("LED switched on");
            },
            'l' => {
                write!(port, "l")?;
                println!("LED switched off");
            },
            'q' => {
                println!("Bye!");
                break;
            }
            _   => continue,
        }
    }

    Ok(())
}
