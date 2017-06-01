# ofxHammingCode

encode / decode *Hamming(7,4)* and *Hamming(31, 26)*

## API

### ofxHammingCode::H74

hamming(7,4) code

#### uint16_t encode(uint8_t rawData)

result data contain two encoded data. those were sliced as upper and lower 4bit data from rawData, and encoded as hamming(7, 4) code.

#### uint8_t decode(uint16_t hammingEncodedData)

decode

#### bool isCorrect(uint16_t hammingEncodedData)

if data is damaged 1bit noise, then return false.

greater than 1bit damage, result is not guaranteed anything.

#### void correct(uint16_t &hammingEncodedData)

correct `hammingEncodedData` if damaged less than 2bit.

### ofxHammingCode::H74::SECDED

hamming(7,4) code with parity check

#### uint16_t encode(uint8_t rawData)
#### uint8_t decode(uint16_t hammingEncodedData)
#### bool isCorrect(uint16_t hammingEncodedData)
#### void correct(uint16_t &hammingEncodedData)

see `ofxHammingCode::H74`

#### bool isCorrectable(uint16_t hammingEncodedData)

if data isn't damaged then return true,

else if data is damaged 1bit noise, then return true,

else if data is damaged 2bit noise, then return false,

else if data is damaged greater than 2bit noise, return value is only GOD knows.

### ofxHammingCode::H3126

hamming(31,26) code

#### uint32_t encode(uint32_t rawData)

`rawData` need be less than `26bit`.

#### uint32_t decode(uint32_t hammingEncodedData)
#### bool isCorrect(uint32_t hammingEncodedData)
#### void correct(uint32_t &hammingEncodedData)

see `ofxHammingCode::H74`

### ofxHammingCode::H3126::SECDED

hamming(31,26) code with parity check

#### uint32_t encode(uint32_t rawData)
#### uint32_t decode(uint32_t hammingEncodedData)
#### bool isCorrect(uint32_t hammingEncodedData)
#### bool isCorrectable(uint32_t hammingEncodedData)
#### void correct(uint32_t &hammingEncodedData)

see `ofxHammingCode::H74` and `ofxHammingCode::H74::SECDED`

## Update history

### 2015/03/29 ver 0.01 release

## License

MIT License.

## Author

* ISHII 2bit [bufferRenaiss co., ltd.]
* ishii[at]buffer-renaiss.com

## Special Thanks

* [wplatzer](https://github.com/wplatzer)

## At the last

Please create new issue, if there is a problem.
And please throw pull request, if you have a cool idea!!
