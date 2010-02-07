#ifndef CRYPTO_H_INCLUDED
#define CRYPTO_H_INCLUDED

#include "globals.h"
#include <botan/botan.h>

class Crypto
{
	public:
	/** Constructor (Chunker)
	 *
	 * initializes botan
	 * sets the blocksize
	 */
	Crypto();

	quint8 getBlockSize();
	/**
	*encrypts data
	* \param _data given QByteArray where to get the cleartext from and write the ciphertext in
	* \param _userKey password which is processed to generate an encryption/decryption key
	*/
	void encrypt(QByteArray& _data, QString _userKey);

	/**
	* decrypts data, pretty the same like encrypt
	* \param _data given QByteArray where to get the ciphertext from and write the cleartext in
	* \param _userKey password which is processed to generate an encryption/decryption key
	*/
	void decrypt(QByteArray& _data, QString _userKey);

	private:
	///en- or decryption key
	QString key;
	///blocksize of the cipher
	quint8 blocksize;
};

#endif
