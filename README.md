# Telemetry Parser  
C++ tool to parse and validate spacecraft telemetry data (simulated). Relevant to NASA mission operations automation.

## Features  
- Parses binary telemetry packets with headers (timestamp, packet ID) and payloads (temperature, pressure, power).  
- Validates checksums to detect data corruption.  
- Handles network-to-host byte order conversion (big-endian).  

## Input Files  
Sample binary files included:  
1. `telemetry_valid.bin` → Valid checksum.  
2. `telemetry_invalid_checksum.bin` → Invalid checksum.  
3. `telemetry_edge_cases.bin` → Extreme sensor values.  

## Build & Run  
```bash
# Clone and build
git clone [your-repo-url]
cd nasa-telemetry-parser
mkdir build && cd build
cmake .. && make

# Run with a sample file
./telemetry_parser ../telemetry_valid.bin
