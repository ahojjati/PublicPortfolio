#include <iostream>
#include "CommonCrypto/CommonCrypto.h"

using namespace std;

char * decrypt(string k, string c)
{
	string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char * result = new char[c.size()];
	c.copy(result,c.size(),0);
	for (int i = 0; i < c.size(); ++i) {
		for (int j = 0; j < k.size(); ++j) {
			if (c[i] == k[j]) {
				result[i] = alpha[j];
				break;
			}
		}
	}
	return result;
}

int main()
{
	/*
	 * END   - XXX
	 * START - Decrypt Substitution Cypher
	 */
	
	string key = "XJOSUTKLHVWEPRBMZQIDCYFGNA";
	string cypher_text = "XPUQHOXR HRSHXR XODHYHID QCIIUEE PUXRI KXYU YBHOU DB DLHI MBFUQTCE OLHUT MBOXLBRDXI MXMX";
	char * temp = decrypt(key,cypher_text);
	
    /*
	 * END   - Decrypt Substitution Cypher
	 * START - Decrypt AES
	 */
	int keylen = 256;
    
	unsigned char aes_key[]   = {0xb8,0x4e,0x81,0xfa,0x46,0xd5,0x99,0x39,0x93,0x5c,0xc9,0xd9,0x2b,0xf4,0x7c,0x34,0xc8,
        0xb2,0x6a,0xe6,0xf3,0x6a,0x87,0x88,0x53,0x6c,0xc0,0x72,0xce,0xae,0x9b,0x6f};
	unsigned char aes_iv[]    = {0x3c,0xcd,0x5e,0x18,0xeb,0x3c,0x72,0xb1,0xaf,0xa3,0x1b,0xcb,0x66,0x32,0xea,0x77};
	unsigned char aes_crypt[] = {0x7d,0xbf,0xd5,0x84,0xf7,0x02,0x80,0x37,0xf3,0x5d,0x04,0x13,0x79,0x91,0x74,0x02,0xc4,
        0x08,0x63,0x33,0x48,0x01,0x04,0x7e,0x7e,0x4d,0xa7,0xb3,0xf9,0x5e,0xfe,0x6f,0xb1,0x46,
        0x24,0xee,0xc1,0x95,0xec,0x4a,0x05,0x43,0x0b,0xa8,0x71,0x49,0x0c,0x4c,0x40,0x9c,0x6f,
        0x5f,0x63,0xd2,0x76,0x9e,0x4a,0x9a,0x4d,0xf5,0x03,0x75,0xbb,0x0f,0x6d,0xd9,0xa2,0x80,
        0x38,0x03,0xb1,0x81,0x3b,0x5b,0x0d,0x9d,0x82,0xca,0x79,0xa0,0x48,0x43,0x71,0x2e,0x72,
        0x85,0x7f,0xf5,0x76,0xdf,0x24,0x2b,0xb4,0x17,0x2c,0xdf};
	unsigned char * aes_plain = new unsigned char[96];
    CCCrypt(kCCDecrypt, kCCAlgorithmAES128, kCCOptionPKCS7Padding, aes_key, keylen, aes_iv, aes_crypt, 96, aes_plain, 96, 0);
    
    /*
     *---------------------------
     *---------------------------
	 * END   - Decrypt AES
	 * START - Break Weak AES
     *---------------------------
     *---------------------------
	 */
    
    
    
    
    
    
    
    
    
    unsigned char weak_cypher[] = {0x68,0x5a,0xb1,0x67,0xef,0xd4,0x05,0x0d,0x45,0x34,0x81,0x8e,0xce,0xf5,0xc3,0x54,0x5c,0xb2,0x3e,0xe1,0x0e,0x9a,0x81,0x25,0xf8,0x22,0xbf,0x1d,0x6e,0x77,0x33,0xea,0x3c,0xb1,0x01,0x7a,0x7e,0xe7,0x8c,0x70,0xd8,0xb8,0x65,0xe5,0x16,0xd0,0x52,0xe3,0x32,0x5f,0x52,0x07,0xc8,0xfc,0x0e,0x37,0x5b,0x57,0xd1,0xbb,0x41,0x22,0x0b,0x00,0x03,0xcc,0x6e,0x15,0xe2,0x0c,0x73,0xc8,0xa7,0xf9,0x91,0xbe,0x62,0xd2,0x8a,0x7f,0x1a,0x88,0xa1,0xf6,0x45,0x48,0xe1,0x81,0x60,0xd3,0x4a,0x8b,0xc2,0xce,0x76,0xf5,0x3b,0x3c,0xb8,0x84,0x2d,0xd2,0x10,0xd8,0xef,0xa5,0x8f,0xbb,0x9d,0xf4,0xca,0x07,0xc0,0x49,0x63,0x82,0x98,0x1d,0x02,0xd1,0x2e,0xb6,0x95,0xa3,0x4b,0x8e,0xf3,0xb5};
    unsigned char weak_key[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    unsigned char weak_iv[] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
    unsigned char * weak_plain[32];
    for (int i = 0; i < 32; ++i) {
        weak_plain[i] = new unsigned char[128];
    }
    for (int i = 0; i < 32; ++i) {
        weak_key[31] += 1;
        CCCrypt(kCCDecrypt, kCCAlgorithmAES128, kCCOptionPKCS7Padding, weak_key, 256, weak_iv, weak_cypher, 128, weak_plain[i], 128, 0);
    }
    for (int i = 0; i < 32; ++i) {
        cout << "key trial number " << i << " : "<< endl;
        for (int j = 0; j < 128; j++) {
            cout << (char) weak_plain[i][j];
        }
        cout << endl;
    }
    
    
    
    
    /*
     *---------------------------
     *---------------------------
	 * END   - Break Weak AES
	 * START - Bonus Substitution
     *---------------------------
     *---------------------------
	 */
    
    
    
    
    
    string bonus_cypher = "MRO LCOH GD PSLMV DGW OUHFAJO JLEO MRHM GD H DLSNOW CGOK SGM HWGPKO MRGPNRM PSJOKK KGFO BGSBOAMLGS GD AJPWHJLMV ZPM QROS MRO GSO LK HJKG MRO GAAGKLMO GD GSO MRO BGSMWHCLBMLGS WLKO MG WODJOBMLGS HS OUHFAJO GD MRLK LK ZV HSV GZTOBM GD KLNRM HJJ SPFZOW HJKG HS OJOXHMLSN ODDOBM LM MRO FLSC GPM GD MRO DGHF HSC DJPU GD NOSOWHMLGS MG MRO BGSMOFAJHMLGS GD ZOLSN JOKKOW FLJLMHWV HSC WOMHLJ HJKG MRO WOMHLJ PKO LK SGM ZV PK ZPM HK GPW NPHWCLHS LK MG ZO H KGJCLOW HK QOJJ HK H ARLJGKGAROW MRO FLJLMHWV GSO FHV ZO HSC MG GPW RLNROW APWAGKO SG KBLOSBO BHS ZO ZOMMOW ZPM LM FPKM ZO LS MRO KALWLM GD H ARLJGKGAROW SGM GD H KRGAEOOAOW LM LK BGSBOWSOC SGM QLMR XLKLZJO ZPM QLMR HZKMWHBM MWPMR DGW HWO APWO MRO MWPO HWLMRFOMLBLHS LSCLNSHSMJV MRHM RLK PSLM LK BHAHZJO GD CLXLKLGS QROS VGP CLXLCO RO MRHM VGP HWO GSJV FPJMLAJVLSN RLK GSO LK SGM FHMOWLHJ GW WOKGJXHZJO LSMG ZPM HS PSXHWVLSN HSC HZKGJPMO OYPHJLMV HSC MRLK MRO APWOJV LSMOJJOBMPHJ BRHWHBMOW GD RLK KMPCV SGMO HJKG MRO NWOHM AGQOW QRLBR HWLMRFOMLB GD MRO SG GMROW CLKBLAJLSO LK OYPHJJV KOXOWO GW HS OYPHJ MOKM GD NOSOWHJ HZLJLMV GW OYPHJJV LFAWGXLSN MG H KMPALC AOWKGS JOM GPW KOBGSC ZWHSBR GD OCPBHMLGS ZO NOGFOMWV L BHS OHKLJV KOO MRHM MRO KELJJ GD MRO NOSOWHJ QLJJ ZO CGPZJOC ZV RLK ESGQJOCNO GD NOGFOMWV MRHM LK H KFHJJ FHMMOW MRO PKO GD NOGFOMWV MG QRLBR L WODOW LK MRO HKKLKMHSBO NLXOS ZV LM LS MRO BGSMOFAJHMLGS GD MRO LCOH GD NGGC HSC MRO BGFAOJJLSN MRO FLSC MG JGGE HM MWPO ZOLSN HSC SGM HM NOSOWHMLGS GSJV VOM MRO AWOKOSM FGCO GD MROKO HK HSV GSO QRG LK MRO JOHKM GD H FHMROFHMLBLHS LK HQHWO LK FOHS HSC WLCLBPJGPK MROV HWO FHCO MG JGGE MG MRO HSC SGM PAQHWCK MG OMOWSHJ OULKMOSBO MRO NOGFOMOW LK HJQHVK MHJELSN GD KYPHWLSN HK LD RO RHC LS XLOQ HBMLGS QROWOHK ESGQJOCNO LK MRO WOHJ GZTOBM GD MRO KMPCV LM KRGPJC OJOXHMO MRO KGPJ HSC BWOHMO MRO FLSC GD ARLJGKGARV LM KRGPJC WHLKO PA QRHM DHJJOS CGQS SGM MG KAOHE GD JOKKOW LS QHW HSC FLJLMHWV MHBMLBK HSC LS MRO LFAWGXOFOSM GD MRO KRHJJ QO AWGAGKO HK H MRLWC ZWHSBR GD GPW OCPBHMLGS HKMWGSGFV XOWV NGGC MRO ESGQJOCNO GD MRO ROHXOSK LK SOBOKKHWV HM GSBO DGW RPKZHSCWV SHXLNHMLGS FLJLMHWV MHBMLBK L JLEO VGPW QHV GD NLXLSN PKODPJ DGW OXOWVMRLSN LS GWCOW MG FHEO GD MRO QGWJC HSC MROWO LK H CLDDLBPJMV LS AWGXLSN MG FHSELSC MRHM OCPBHMLGS LK SGM GSJV PKODPJ LSDGWFHMLGS ZPM H APWLDLBHMLGS GD MRO OVO GD MRO KGPJ QRLBR LK ZOMMOW MRHS MRO ZGCLJV OVO DGW ZV MRLK HJGSO LK MWPMR KOOS SGQ QLJJ VGP HAAOHJ MG FHSELSC LS NOSOWHJ GW MG MRO ARLJGKGAROW GW QGPJC VGP AWODOW MG JGGE MG VGPWKOJD GSJV OXOWV FHS LK RLK GQS ZOKM DWLOSC MROS MHEO H KMOA ZHBEQHWC DGW QO HWO GPM GD GWCOW HSC LSKOWM MRO MRLWC CLFOSKLGS QRLBR LK GD HDMOW MRO KOBGSC QRLBR";
    int alpha_dist[26];
    for (int i = 0; i < 26; i++) {
        alpha_dist[i] = 0;
    }
    for (int i = 0; i < bonus_cypher.size(); i++) {
        if (bonus_cypher[i] != ' ') {
            alpha_dist[bonus_cypher[i] - 'A'] ++;
        }
    }
//    cout << "-----------------------------------------------------" << endl;
//    cout << "Alphabet Distribution for bonus cypher is as follows:" << endl;
//    cout << "-----------------------------------------------------" << endl;
    for (int i = 0; i < 26; i++) {
        //cout << (char)('A'+i) << " : " << alpha_dist[i] << endl;
    }
    int max = 25;
    char alpha[26];
    //started with standard frequency of english language then manipulated based on known words in text.
    char alpha_real[26] = {'E','T','O','I','A','N','S','R','H','L','U','D','F','Y','C','P','M','B','G','W','V','K','Q','X','J','Z'};
    for (int i = 0; i < 26; i++) {
        for (int j = 0; j < 26; j++) {
            if (alpha_dist[j] > alpha_dist[max]) {
                max = j;
            }
        }
        alpha_dist[max] = -1;
        alpha[i] = (char)('A'+max);
    }
    //cout << "-----------------------------------------------------" << endl;
    //cout << "  Alphabet Ordering for bonus cypher is as follows:" << endl;
    //cout << "-----------------------------------------------------" << endl;
    for (int i = 0; i < 26; i++) {
//        cout << alpha[i] << " , ";
    }
    //cout << endl;
    char * bonus_result = new char[bonus_cypher.size()];
    bonus_cypher.copy(bonus_result, bonus_cypher.size());
    for (int i = 0; i < bonus_cypher.size(); i++) {
        for (int j = 0; j < 26; j++) {
            if (bonus_result[i] == alpha[j]) {
                bonus_result[i] = alpha_real[j];
                break;
            }
        }
    }
    //cout << bonus_result <<endl;
    delete bonus_result;
    delete aes_plain;
	return 0;
}