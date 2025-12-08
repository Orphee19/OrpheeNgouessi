#include <src/SerialNumber.h>

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>

#include <openssl/aes.h>
#include <openssl/sha.h>
#include <iomanip>
#include <iostream>


namespace ILogServer
{
void handleErrors(void)
{
    ERR_print_errors_fp(stderr);
    abort();
}

bool SerialNumber::createSerialNumber(const std::string& macAddress, const std::string& deviceType, std::string& output)
{
    if (macAddress.empty() or deviceType.empty())
    {
        return false;
    }

    std::string toEncrypt = macAddress + "-" + deviceType + "-" + "I2TECH-IoTServer";

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
}