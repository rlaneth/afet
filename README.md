# afet

> :warning: **WARNING:** this program is for educational purposes only. It is
> NOT safe to use it for encryption of sensitive data.

**afet** (an acronym for _“a file encryption tool”_) is a tiny file encryption
program based on the ChaCha algorithm.

As a toy project developed exclusively as an exercise on understanding
[RFC 7539][rfc-7539], it is lacking in many ways and **MUST NOT** be considered
secure.

### Known issues

- afet has no means of checking message integrity on decryption. This means that
  the contents of an encrypted file might be changed (e.g. by an attacker) and
  it will still decrypt successfully. The [Poly1305][rfc-8439] MAC is usually
  used with ChaCha to avoid this issue, but it is not implemented here.

- The user interface is not well polished. Notably, there are issues related to
  parsing command line arguments and the program lacks a “help” option.

- The program has in no way been extensively tested, meaning a high change of
  severe bugs, including those which could leave the encrypted information at
  risk.

### Build

Just run `make` from the repository root. The compiled binary will be placed in
the `bin` directory.

### Usage

```
$ afet COMMAND -i INPUT -o OUTPUT -l LIMIT KEYFILE NONCE
```

- `INPUT` is only valid for the `encrypt` and `decrypt` commands, and determines
  the path of the plaintext file to be encrypted;

- `OUTPUT` is an optional parameter which determines the path for a file to
  which the output of the command should be written;

- `LIMIT` is only valid for the `random` command and determines the maximum
  number of random bytes to be generated (default is 18,446,744,073,709,551,615,
  the maximum possible value);

- `KEYFILE` must be the path to a file exactly 32 bytes in length;

- `NONCE` is the nonce to be used in the encryption algorithm and must be an
  integer between 0 and 18,446,744,073,709,551,615.

#### Random

Generates random data based on a key and a nonce. Example:
```
$ afet keyfile 1
```

#### Encryption

Encrypts a file based on the random generator output.
```
$ afet encrypt -i plaintext.txt -o ciphertext.bin key.bin 1
```

#### Decrypt

> :information_source: **Note:** as the encryption process is a simple XOR, the
> `decrypt` command is actually an alias to `encrypt`.

Decrypts a file based on the random generator output.
```
$ afet decrypt -i ciphertext.bin -o plaintext.bin key.bin 1
```

[rfc-7539]: https://datatracker.ietf.org/doc/html/rfc7539
[rfc-8439]: https://datatracker.ietf.org/doc/html/rfc8439