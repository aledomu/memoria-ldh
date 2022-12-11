use console::Term;
use std::{io::{Read, Write}, error::Error, time::Duration};

const PORT_PATH: &str = "/dev/ttyACM0";
const BAUD_RATE: u32 = 9_600;

struct MeasureData {
    resistor_volts: f32,
    ldr_lux: f32,
}

impl MeasureData {
    fn from_reader<R: Read>(reader: &mut R) -> Result<Self, std::io::Error> {
        let mut buf = [0; 4];

        Ok(Self {
            resistor_volts: {
                reader.read_exact(&mut buf)?;
                f32::from_le_bytes(buf)
            },
            ldr_lux: {
                reader.read_exact(&mut buf)?;
                f32::from_le_bytes(buf)
            }
        })
    }
}

fn main() -> Result<(), Box<dyn Error>> {
    let mut out_port = serialport::new(PORT_PATH, BAUD_RATE)
        .timeout(Duration::from_secs(60))
        .open_native()?;

    let mut in_port = out_port.try_clone_native()?;
    std::thread::spawn(move || {
        loop {
            if let Ok(data) = MeasureData::from_reader(&mut in_port) {
                // Por alguna razon hay que introducir el codigo CR manualmente
                println!("{} V, {} lux\r", data.resistor_volts, data.ldr_lux);
            }
        }
    });
    
    println!("Temperature measures from Arduino, press 't' to query temperature, 'q' to quit:");

    let t = Term::stdout();
    loop {
        match t.read_char()? {
            't' => write!(out_port, "t").unwrap(),
            'q' => break,
            _   => continue,
        }
    }

    Ok(())
}
