#include "Crypto.h"
#include <QDebug>

Crypto::Crypto() {
	Botan::LibraryInitializer init;
	blocksize = 16;
	//TODO no salt in the encryption process, would that be better?
}

quint8 Crypto::getBlockSize()
{
	return blocksize;
}

void Crypto::encrypt(QByteArray& _data, QString _userKey)
{
	//TODO refactor en- and decryption into one method or call a general internal method from both

	//first hash the userkey and generate another init. vector from it
	Botan::HashFunction* hash = Botan::get_hash("SHA-256");
	Botan::SymmetricKey skey = hash->process(_userKey.toStdString());
	Botan::SecureVector<Botan::byte> raw_iv = hash->process('0'+_userKey.toStdString());

	Botan::InitializationVector iv(raw_iv, blocksize);
	//set up the encryption pipe where data can go through
	Botan::Pipe pipe(get_cipher("Twofish/XTS", skey, iv, Botan::ENCRYPTION));

	//create a stdstring from the data because botan only takes it this way
	std::string nextstring(_data.data(),_data.length());

	//process the pipe
	pipe.process_msg(nextstring); 

	nextstring = pipe.read_all_as_string();
	//create a temporary var to get the ciphertext back into the QByteArray
	QByteArray test2(nextstring.c_str(),nextstring.length());
	_data = test2;

}

void Crypto::decrypt(QByteArray& _data, QString _userKey)
{
	//the same like the encrypt method, only use decryption pipe
	Botan::HashFunction* hash = Botan::get_hash("SHA-256");
	Botan::SymmetricKey skey = hash->process(_userKey.toStdString());
	Botan::SecureVector<Botan::byte> raw_iv = hash->process('0'+_userKey.toStdString());

	Botan::InitializationVector iv(raw_iv, blocksize);
	Botan::Pipe pipe(get_cipher("Twofish/XTS", skey, iv, Botan::DECRYPTION));

	std::string nextstring(_data.data(),_data.length());

	pipe.process_msg(nextstring);

	nextstring = pipe.read_all_as_string();
	QByteArray test2(nextstring.c_str(),nextstring.length());
	_data = test2;
}
