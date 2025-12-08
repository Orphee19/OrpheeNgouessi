#include <src/SerialNumber.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/aes.h>
#include <openssl/sha.h>
#include <iomanip>

#include <iostream>
#include <sstream>

typedef unsigned char byte;

namespace CarflowServer
{
bool SerialNumber::createSerialNumber(const std::string& macAddress, const std::string& deviceType, std::string& output)
{
    if (macAddress.empty() or deviceType.empty())
    {
        return false;
    }

    std::string toEncrypt = macAddress + "-" + deviceType + "-" + "I2TECH-Carflow";

    EVP_MD_CTX *mdCtx = EVP_MD_CTX_new();
    unsigned char mdVal[SHA256_DIGEST_LENGTH];
    unsigned int mdLen;

    if (!EVP_DigestInit_ex(mdCtx, EVP_sha256(), nullptr))
    {
        printf("Message digest initialization failed.\n");
        EVP_MD_CTX_free(mdCtx);
        return false;
    }

    // Hashes cnt bytes of data at d into the digest context mdCtx
    if (!EVP_DigestUpdate(mdCtx, toEncrypt.c_str(), toEncrypt.length()))
    {
        printf("Message digest update failed.\n");
        EVP_MD_CTX_free(mdCtx);
        return false;
    }

    if (!EVP_DigestFinal_ex(mdCtx, mdVal, &mdLen))
    {
        printf("Message digest finalization failed.\n");
        EVP_MD_CTX_free(mdCtx);
        return false;
    }

    EVP_MD_CTX_free(mdCtx);

    std::stringstream ss;
    for(unsigned int i = 0; i < mdLen; i++){
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>( mdVal[i] );
    }

    output = ss.str();

    return true;
}

/*void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

bool SerialNumber::createSerialNumber(const std::string& macAddress, const std::string& deviceType, const std::string& key, std::string& output)
{
    if (macAddress.empty() or deviceType.empty() or key.empty())
    {
        return false;
    }

    std::string toEncrypt = macAddress + "-" + deviceType + "-" + "I2TECH-Carflow";

    EVP_CIPHER_CTX *ctx;
    int len;
    int ciphertext_len;

    unsigned char iv[AES_BLOCK_SIZE] = {0};
    output.resize(toEncrypt.size() + AES_BLOCK_SIZE);

    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, (const byte*)&key[0], iv))
        handleErrors();


    if(1 != EVP_EncryptUpdate(ctx, (byte*)&output[0], &len, (const byte*)&toEncrypt[0], toEncrypt.length()))
        handleErrors();

    ciphertext_len = len;

    if(1 != EVP_EncryptFinal_ex(ctx, (byte*)&output + len, &len))
        handleErrors();
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return ciphertext_len != 0;
}

bool SerialNumber::decryptSerialNumber(const std::string& serialNumber, const std::string& key, std::string& output)
{
   if (serialNumber.empty() or key.empty())
    {
        return false;
    }

    EVP_CIPHER_CTX *ctx;

    int len;
    int plaintext_len;

    if(!(ctx = EVP_CIPHER_CTX_new()))
        handleErrors();

    unsigned char iv[AES_BLOCK_SIZE] = {0};

    if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, (const byte*)&key[0], iv))
        handleErrors();

    if(1 != EVP_DecryptUpdate(ctx, (byte*)&output[0], &len, (const byte*)&serialNumber[0], serialNumber.length()))
        handleErrors();
    plaintext_len = len;

    if(1 != EVP_DecryptFinal_ex(ctx, (byte*)&output + len, &len))
        handleErrors();

    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len != 0;

    //AES_cbc_encrypt((unsigned char*)toCrypt.c_str(), enc_out, toCrypt.length(), &enc_key, iv, AES_DECRYPT);
}*/
}