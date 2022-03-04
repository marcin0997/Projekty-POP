#include <iostream>
#include <fstream>
#include <vector>

using namespace std;


#pragma pack(push, 1)
struct BITMAPFILEHEADER{
    short bfType;
    int bfSize;
    short bfReserved1;
    short bfReserved2;
    int bfOffBits;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct BITMAPINFOHEADER{
    int biSize;
    int biWidth;
    int biHeight;
    short biPlanes;
    short biBitCount;
    int biCompression;
    int biSizeImage;
    short biXPelsPerMeter;
    short biYPelsPerMeter;
    int biClrUsed;
    int biClrImportant;
};
#pragma pack(pop)
#pragma pack(push, 1)
struct Pixel{
    int r,g,b;
};
#pragma pack(pop)

/**
 *
 * @param str informacja o błędzie
 */
void error(string str){
    cerr<<"ERR: "<<str<<endl;
    exit(-1);
}
class Bitmap {
    string input, output;
    BITMAPFILEHEADER fileheader;
    BITMAPINFOHEADER infoheader;
    char *pixels;

public:
/**
 *
 * @param s nazwa pliku wejsciowego
 * @param o nazwa pliku wyjsciowego
 */
    Bitmap(string s, string o) {
        input=s;
        output=o;
        ifstream f(s, ios::binary);
        if(!f)error("Plik nie istnieje!");
        f.read((char *) &fileheader, sizeof(BITMAPFILEHEADER));
        f.read((char *) &infoheader, sizeof(BITMAPINFOHEADER));
        if(infoheader.biCompression!=0)error("Plik nie moze byc skompresowany!");
        if(infoheader.biBitCount!=24)error("Obraz musi byc 24 bitowy!");
        f.seekg(fileheader.bfOffBits, ios::beg);
        int pixelSize = fileheader.bfSize - fileheader.bfOffBits;
        pixels = new char[pixelSize];
        f.read((char *) &pixels[0], pixelSize);
        char *tmp = new char[infoheader.biWidth * infoheader.biHeight * sizeof(Pixel)];
        Pixel *arr = (Pixel *) tmp;
        for (int y = 0; y < infoheader.biHeight; y++) {
            for (int x = 0; x < infoheader.biWidth; x++) {
                arr->b = *(pixels++);
                arr->g = *(pixels++);
                arr->r = *(pixels++);
                arr++;
            }
        }
        pixels = tmp;
        f.close();
    }
    /**
     * metoda zamieniająca piksele na osi x
     */
    void hmirror() {
        //rzutowanie tablicy zawierającej informacje o kolorze w kazdej komórce, do tablicy pikseli zawierającej informacje o każdym pikselu
        Pixel *pixs = (Pixel *) pixels;
        Pixel *tmp = new Pixel[infoheader.biWidth * infoheader.biHeight];
        //kopiowanie informacji o pikselach do tymczasowej tablicy dynamicznej
        for (int y = 0; y < infoheader.biHeight; y++) {
            for (int x = 0; x < infoheader.biWidth; x++) {
                if (y < infoheader.biHeight && y > 0 && x < infoheader.biWidth && x > 0) {
                    tmp[y * infoheader.biWidth + x] = pixs[y * infoheader.biWidth + x];
                }
            }
        }
        //zamiana miejscami pikseli na osi x z tablicy tymczasowej do wlasciwej tablicy pikseli
        for (int y = 0; y < infoheader.biHeight; y++) {
            for (int x = 0; x < infoheader.biWidth; x++) {
                if (y < infoheader.biHeight && y > 0 && x < infoheader.biWidth && x > 0) {
                    pixs[y * infoheader.biWidth + (x)] = tmp[y * infoheader.biWidth + infoheader.biWidth - x - 1];
                }
            }
        }
        //rzutowanie tablicy pikseli to tablicy zawierającej informacje o każdym pikselu w osobnej komórce
        pixels = (char *) pixs;
        delete tmp;
    }

    /**
     *
     * @param s nazwa pliku do którego ma zostac zapisany przetworzony obraz
     */
    void save(string s) {
        ofstream f(s, ios::binary);
        f.write((char *) &fileheader, sizeof(BITMAPFILEHEADER));
        f.write((char *) &infoheader, sizeof(BITMAPINFOHEADER));
        f.seekp(fileheader.bfOffBits, ios::beg);
        char *tmp = new char[infoheader.biWidth * infoheader.biHeight * sizeof(Pixel)];
        char *o = tmp;
        Pixel *arr = (Pixel *) pixels;
        for (int y = 0; y < infoheader.biHeight; y++) {
            for (int x = 0; x < infoheader.biWidth; x++) {
                *(o++) = (arr)->b;
                *(o++) = (arr)->g;
                *(o++) = (arr)->r;
                arr++;
            }
        }
        f.write((char*)&tmp[0],fileheader.bfSize-fileheader.bfOffBits);
        delete [] tmp;
    }
    void save(){save(output);};

};



int main(int argv, char **argc) {
    Bitmap *bitmap = new Bitmap("piers_termogram_spectrum.bmp","piers_termogram_spectrum_odbicie.bmp");
    bitmap->hmirror();
    bitmap->save();
    delete bitmap;

}


