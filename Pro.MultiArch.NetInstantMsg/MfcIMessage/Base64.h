
static unsigned char base64EncodeChars[] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
        'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
        'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
        'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
        'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
        'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
        'w', 'x', 'y', 'z', '0', '1', '2', '3',
        '4', '5', '6', '7', '8', '9', '+', '/'};
//61
static unsigned char base64DecodeChars[] = {
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
        -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
        15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
        -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1};

class Base64 {

public:
	static CString Encode(CString unicode) {
		CString bytes;
		for (int i=0;i<unicode.GetLength();i++) {
			unsigned ch=unicode[i];
			if (ch < 0x80u) {
				bytes+=TCHAR(ch);
			} else if (ch < 0x800u) {
				bytes+=TCHAR((ch>>6)^0xC0);
				bytes+=TCHAR((ch&63)^0x80);
			} else {
				bytes+=TCHAR((ch>>12)^0xE0);
				bytes+=TCHAR(((ch>>6)&63)^0x80);
				bytes+=TCHAR((ch&63)^0x80);
			}
		}
		CString base64;
		for (int i=0;i<bytes.GetLength();i+=3) {
			unsigned c1=bytes[i];
			base64+=(TCHAR)base64EncodeChars[c1>>2];
			if (i+1<bytes.GetLength()) {
				unsigned c2=bytes[i+1];
				base64+=(TCHAR)base64EncodeChars[((c1&3)<<4)|(c2>>4)];
				if (i+2<bytes.GetLength()) {
					unsigned c3=bytes[i+2];
					base64+=(TCHAR)base64EncodeChars[((c2&15)<<2)|(c3>>6)];
					base64+=(TCHAR)base64EncodeChars[c3&63];
				} else {
					base64+=(TCHAR)base64EncodeChars[(c2&15)<<2];
					base64+=(TCHAR)'=';
				}
			} else {
				base64+=(TCHAR)base64EncodeChars[(c1&3)<<4];
				base64+=(TCHAR)'=', base64+=(TCHAR)'=';
			}
		}
		return base64;
	}

	static CString Decode(CString base64) {
		CString bytes;
		for (int i=0;i+4<=base64.GetLength();i+=4) {
			unsigned c1=base64DecodeChars[base64[i]],c2=base64DecodeChars[base64[i+1]];
			bytes+=TCHAR((c1<<2)|(c2>>4));
			if (base64[i+2]=='=')
				break;
			else {
				unsigned c3=base64DecodeChars[base64[i+2]];
				bytes+=TCHAR(((c2&15)<<4)|(c3>>2));
				if (base64[i+3]=='=')
					break;
				else {
					unsigned c4=base64DecodeChars[base64[i+3]];
					bytes+=TCHAR(((c3&3)<<6)|c4);
				}
			}
		}
		CString unicode;
		for (int i=0;i<bytes.GetLength();) {
			if (!(bytes[i]&0x80)) {
				unicode+=TCHAR(bytes[i]);
				++i;
			} else if (!(bytes[i]&0x20)) {
				unicode+=TCHAR(((bytes[i]^0xC0)<<6)|(bytes[i+1]^0x80));
				i+=2;
			} else {
				unicode+=TCHAR(((bytes[i]^0xE0)<<12)|(bytes[i+1]^0x80)<<6|(bytes[i+2]^0x80));
				i+=3;
			}
		}
		return unicode;
	}
};