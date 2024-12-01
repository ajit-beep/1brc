#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <memory>
#include <iomanip>



class FileReader {
public:
    std::ifstream m_ifst;
    static const int BUFFER_SIZE = 64 * 1024 * 1024;

    std::vector<char> m_buffer;

    FileReader(const std::string& fileName) : m_buffer(BUFFER_SIZE) {
        m_ifst.open(fileName, std::ios::in | std::ios::binary);

        if(!m_ifst){
            std::cerr << "Error opening in File: " << fileName << std::endl;
            exit(1);
        }

    }
    
    void read();

};

struct data{
    double min;
    double max;
    double sum;
    uint64_t count;
};

std::unordered_map<std::string, data> g_data;

void FileReader::read() {
    std::string residual;

    while(!m_ifst.eof()){
        m_ifst.read(m_buffer.data(), BUFFER_SIZE);

        std::streamsize bytes_read = m_ifst.gcount();

        std::string chunk(m_buffer.data(), bytes_read);

        if(!residual.empty()) {
            chunk = residual + chunk;
            residual.clear();
        }

        size_t pos = 0;
        while(true) {
            size_t next_line = chunk.find('\n', pos);
            if(next_line == std::string::npos) {
                residual = chunk.substr(pos);
                break;
            }

            std::string line = chunk.substr(pos, next_line - pos);
            pos = next_line + 1;

            // parse the line
            size_t sep_pos = line.find(';');
            if(sep_pos != std::string::npos) {
                std::string station = line.substr(0, sep_pos);
                double temp = std::stod(line.substr(sep_pos+1));

                auto& entry = g_data[station];
                if(entry.count == 0){
                    entry.max = entry.min = temp;
                    entry.sum = temp;
                    entry.count++;
                }else {
                    if(temp < entry.min) entry.min = temp;
                    if(temp > entry.max) entry.max = temp;
                    entry.sum += temp;
                    entry.count++;
                }
            } else {
                //ignore malformed lines;
            }
        }
    }

    if(!residual.empty()) {
        std::string line = residual;
        size_t sep_pos = line.find(';');
        if(sep_pos != std::string::npos) {
            std::string station = line.substr(0, sep_pos);
            double temp = std::stod(line.substr(sep_pos+1));

            auto& entry = g_data[station];
            if(entry.count == 0){
                entry.max = entry.min = temp;
                entry.sum = temp;
                entry.count++;
            }else {
                if(temp < entry.min) entry.min = temp;
                if(temp > entry.max) entry.max = temp;
                entry.sum += temp;
                entry.count++;
            }
        } else {
            //ignore malformed lines;
        }
    }

    m_ifst.close();
}


int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: ./prorgam <datafile>" << std::endl;
        return 1;
    }

    FileReader reader(argv[1]);
    reader.read();

    // collect and sort station names

    std::vector<std::string> stations;

    stations.reserve(g_data.size());
    for (const auto& pair: g_data) {
        stations.push_back(pair.first);
    }

    std::sort(stations.begin(), stations.end());

    // Before the output loop
    std::cout << "{" << std::fixed << std::setprecision(1);

    bool first = true;

    for (const auto& station : stations) {
        if(!first) {
            std::cout << ", ";
        } else {
            first = false;
        }
        
        const auto& entry = g_data[station];
        double mean = entry.sum / entry.count;
        mean = std::round(mean * 10.0) / 10.0; // Round to 1 decimal place

        std::cout << std::fixed << std::setprecision(1)
          << station << "=" << entry.min << "/" << mean << "/" << entry.max;

    }

    std::cout << "}" << std::endl;

    return 0;
}
