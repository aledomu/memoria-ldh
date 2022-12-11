use console::Term;
use std::{error::Error, io::Write, process::exit};

const PORT_PATH: &str = "/dev/ttyACM0";
const BAUD_RATE: u32 = 9_600;

fn main() -> Result<(), Box<dyn Error>> {
    let mut port = serialport::new(PORT_PATH, BAUD_RATE).open_native()?;

    let term = Term::stdout();
    ctrlc::set_handler(|| exit(0))?;
    loop {
        let ch = term.read_char()?;
        write!(port, "{ch}")?;
    }

    #[allow(unreachable_code)]
    Ok(())
}
