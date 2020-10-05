#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>

#include <bencoding.h>

using namespace bencoding;

void TestForBDict();
void TestForBList();
void HowToEncodeDecode();

int main() {
    try {
        TestForBDict();
        TestForBList();
        HowToEncodeDecode();
        return 0;
    } catch (const DecodingError &ex) {
        // There was an error during the decoding.
        std::cerr << "error: " << ex.what() << "\n";
        return 1;
    }
}

void TestForBDict() {
    std::cout << "Test for BDictionary" << '\n';
    // 手动build一个bencode编码
    std::shared_ptr<BDictionary> bDictionary(
        BDictionary::create({
            { BString::create("announce"), BString::create("http://tracker.com") },
            { BString::create("created by"), BString::create("KTorrent 2.1.4") },
            { BString::create("creation date"), BInteger::create(1182163277) },
            { BString::create("info"),
                BDictionary::create({
                    { BString::create("length"), BInteger::create(6)},
                    { BString::create("name"), BString::create("file.txt")},
                    { BString::create("piece length"), BInteger::create(32768)},
                    { BString::create("pieces"), BString::create("]����F�n�^3mTwz}")} 
                })
            }, // also suppoer auto d = BDictionary::create(); d[BItem] = Bitem;
        })
    );
    std::string hand_repr = getPrettyRepr(bDictionary);
    std::cout << "handwrite (" << hand_repr.size() << ")\n"
        << hand_repr << '\n';
    std::cout << "announce : " << std::static_pointer_cast<BString>((*bDictionary)[BString::create("announce")])->value() << std::endl;
}

void TestForBList() {
    std::cout << "Test for BList" << '\n';
    std::shared_ptr<BDictionary> bDictionary(
        BDictionary::create({
            { BString::create("announce"), BString::create("http://tracker.com") },
            { BString::create("created by"), BString::create("KTorrent 2.1.4") },
            { BString::create("creation date"), BInteger::create(1182163277) },
            { BString::create("info"),
                BDictionary::create({
                    { BString::create("length"), BInteger::create(6)},
                    { BString::create("name"), BString::create("file.txt")},
                    { BString::create("piece length"), BInteger::create(32768)},
                    { BString::create("pieces"), BString::create("]����F�n�^3mTwz}")} 
                })
            }, // also suppoer auto d = BDictionary::create(); d[BItem] = Bitem;
            { BString::create("announce-list"),
                BList::create({
                    { BString::create("http://tracker1.com") },
                    { BString::create("http://tracker2.com") },
                    { BString::create("http://tracker3.com") },
                })
            },
        })
    );
    std::string hand_repr = getPrettyRepr(bDictionary);
    std::cout << "handwrite (" << hand_repr.size() << ")\n"
        << hand_repr << '\n';
}

// Encode : BItem object -> raw string with function "string encode(shared_ptr<BItem>)"
// Decode : raw string -> BItem object with function "unique_ptr<BItem> decode(string data)"
// Visualization: BItem object -> human readable string with funciton "string getPrettyRepr(shared_ptr<BItem>)"
void HowToEncodeDecode() {
    std::cout << "HowToEncodeDecode" << '\n';
    std::ifstream infile;
    std::ofstream outfile;
    std::string rawString;
    std::string prettyString;
    std::string bString;
    std::string ret;
    std::shared_ptr<BItem> pBOjbect;

    infile.open("../test/sample1.torrent");
    getline(infile, rawString);
    infile.close();
    std::cout << rawString << std::endl;
    pBOjbect = decode(rawString);
    prettyString = getPrettyRepr(pBOjbect);
    std::cout << "sample 1 (" << prettyString.size() << ")\n"
        << prettyString << '\n';
    bString = encode(pBOjbect);
    ret = (bString == rawString) ? "Yes" : "No";
    std::cout << " Does raw string keep the same after convertion from raw->object->raw ? "
        << ret << "\n\n";
    outfile.open("../test/output1.torrent");
    outfile << bString;
    outfile.close();

    infile.open("../test/sample2.torrent");
    getline(infile, rawString);
    infile.close();
    pBOjbect = decode(rawString);
    prettyString = getPrettyRepr(pBOjbect);
    std::cout << "sample 2 (" << prettyString.size() << ")\n"
        << prettyString << '\n';
    bString = encode(pBOjbect);
    ret = (bString == rawString) ? "Yes" : "No";
    std::cout << " Does raw string keep the same after convertion from raw->object->raw ? "
        << ret << "\n\n";
    outfile.open("../test/output2.torrent");
    outfile << bString;
    outfile.close();

    infile.open("../test/sample3.torrent");
    getline(infile, rawString);
    infile.close();
    pBOjbect = decode(rawString);
    prettyString = getPrettyRepr(pBOjbect);
    std::cout << "sample 3 (" << prettyString.size() << ")\n"
        << prettyString << '\n';
    bString = encode(pBOjbect);
    ret = (bString == rawString) ? "Yes" : "No";
    std::cout << " Does raw string keep the same after convertion from raw->object->raw ? "
        << ret << '\n';
    outfile.open("../test/output3.torrent");
    outfile << bString;
    outfile.close();

}