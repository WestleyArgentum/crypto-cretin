
void Encrypt(std::ofstream& oStream, std::ifstream& iStream, const mpz_ptr N, const mpz_ptr e, unsigned ChunkSize = 5);
void Decrypt(std::ofstream& oStream, std::ifstream& iStream);