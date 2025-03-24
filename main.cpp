#include <iostream>
#include <fstream>
#include <cstdint>
#include <arpa/inet.h>
#include <cstring>

#pragma pack(push, 1)
struct TelemetryHeader {
    uint32_t timestamp;  // elapsed time (4 bytes)
    uint16_t packet_id;   // packet identifier (2 bytes)
};

struct TelemetryPayload {
    float temperature;    // sensor data (4 bytes)
    float pressure;       // in kilopascals
    float power_level;    // in volts
    uint8_t checksum;     
};

struct TelemetryPacket {
    TelemetryHeader header;
    TelemetryPayload payload;
};
#pragma pack(pop)

float ntohf(uint32_t network) {
    uint32_t host_long = ntohl(network);
    float f;
    memcpy(&f, &host_long, sizeof(f));
    return f;
}

void generate_sample_binary_file() {
    TelemetryPacket packet;
    
    packet.header.timestamp = 123456789;
    packet.header.packet_id = 1001;
    packet.payload.temperature = 25.5f;
    packet.payload.pressure = 50.0f;
    packet.payload.power_level = 156.0f;
    
    const uint8_t* temp_bytes = reinterpret_cast<const uint8_t*>(&packet.payload.temperature);
    packet.payload.checksum = temp_bytes[0] + temp_bytes[1] + temp_bytes[2];

    packet.header.timestamp = htonl(packet.header.timestamp);
    packet.header.packet_id = htons(packet.header.packet_id);

    std::ofstream file("telemetry.bin", std::ios::binary);
    file.write(reinterpret_cast<const char*>(&packet), sizeof(packet));
}

void parse_telemetry(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open telemetry file" << std::endl;
        return;
    }

    TelemetryPacket packet;
    file.read(reinterpret_cast<char*>(&packet), sizeof(packet));

    uint32_t timestamp = ntohl(packet.header.timestamp);
    uint16_t packet_id = ntohs(packet.header.packet_id);

    float temperature, pressure, power_level;
    memcpy(&temperature, &packet.payload.temperature, sizeof(float));
    memcpy(&pressure, &packet.payload.pressure, sizeof(float));
    memcpy(&power_level, &packet.payload.power_level, sizeof(float));

    const uint8_t* temp_bytes = reinterpret_cast<const uint8_t*>(&packet.payload.temperature);
    uint8_t calculated_checksum = temp_bytes[0] + temp_bytes[1] + temp_bytes[2];
    bool is_valid = (calculated_checksum == packet.payload.checksum);

    std::cout << "=== Telemetry Packet ===" << std::endl;
    std::cout << "Timestamp: " << timestamp << " ms\n";
    std::cout << "Packet ID: 0x" << std::hex << packet_id << std::dec << "\n";
    std::cout << "Temperature: " << temperature << " °C\n";
    std::cout << "Pressure: " << pressure << " kPa\n";
    std::cout << "Power: " << power_level << " V\n";
    std::cout << "Checksum: " << (is_valid ? "VALID" : "INVALID") << std::endl;
}

int main() {
    generate_sample_binary_file();
    parse_telemetry("telemetry.bin");
    return 0;
}
