# BinaryLove2
BinaryLove2 is a simple header-only binary parsing library. 

# Samples and Documentation
Please check out samples/ folder to see propper usage of the library.

```cpp
int main()
{
    std::ifstream ifile("Magical Moonlight - Pierro Umiliani.wav", std::ios::binary | std::ios::ate);
    if (!ifile.is_open()) return -1;

    ifile.seekg(0, std::ios::end);
    std::streamsize size = ifile.tellg();
    ifile.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!ifile.read(buffer.data(), size)) return -1;


    ifile.close();

    typedef
        layout <
            group<                                  // The "RIFF" chunk descriptor
                std::array<char, 4>,                // Chunk ID
                uint32_t,                           // Chunk Size
                int32_t                             // Format
            >,
            group<                                  // The "fmt" sub-chunk
                std::array<char, 4>,                // Subchunk1 ID
                uint32_t,                           // Subchunk1 Size
                uint16_t,                           // Audio Format
                uint16_t,                           // Num Channels
                uint32_t,                           // Sample Rate
                uint32_t,                           // Byte Rate
                uint16_t,                           // Block Align
                uint16_t                            // Bits Per Sample
            >,
            group<                                  // The "data" sub-chunk
                std::array<char, 4>,                // Subchunk 2 ID
                loop_iteration_count<0, uint32_t>,  // Subchunk 2 Size 
                loop<0, uint8_t>                    // Data
            >
        > wav_layout_t;

    wav_layout_t wav_file;

    wav_file.parse(buffer);

    uint32_t num_channels = wav_file.get<1>().get<3>();
    float duration =
        // Access first element of loop iteration count
        (float)wav_file.get<2>().get<1>().get<0>() /
        ((float)wav_file.get<1>().get<5>());

    std::printf("This wav file has %i channels.\nAudio duration is %fs\n", num_channels, duration);

    std::vector<char> out_buffer = wav_file.unparse<char>();

    std::ofstream ofile("out.wav", std::ios::binary | std::ios::trunc);
    if (ofile.bad()) return -1;
    if (!ofile.write(out_buffer.data(), out_buffer.size())) return -1;
    
    return 0;
}

```
