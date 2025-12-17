#include <iostream>
#include <vector>
#include <string>
#include <chrono>   
#include <iomanip> 
#include <algorithm> 

using namespace std;
using namespace std::chrono;

struct Kuda {
    int id;
    string nama;
    double kecepatanDasar;   
    double staminaMaks;       
    
    double posisiSaatIni;    
    double staminaSaatIni;  
    double langkahDitempuh;  
};

struct RiwayatAnalisis {
    int jarak;
    double waktuRekursif;
    double waktuIteratif;
};

vector<Kuda> dapatkanDataKuda() {
    return {
        {1,  "Symboli Kris S",  19.5, 160.0},
        {2,  "Mejiro Ryan",     18.0, 180.0},
        {3,  "Narita Brian",    21.0, 130.0},
        {4,  "Winning Ticket",  17.5, 190.0},
        {5,  "Symboli Rudolf",  20.0, 155.0},
        {6,  "Goldship",        17.0, 250.0},
        {7,  "Gold City",       18.2, 165.0},
        {8,  "Fenomeno",        17.8, 175.0},
        {9,  "Tokai Teio",      19.8, 140.0},
        {10, "Special Week",    18.5, 170.0},
        {11, "Still In Love",   16.5, 200.0},
        {12, "Oguri Cap",       19.0, 160.0},
        {13, "Mahantan Caffe",  16.0, 240.0},
        {14, "Agnes Tachyon",   19.2, 145.0},
        {15, "Jungle Pocket",   18.8, 150.0},
        {16, "Super Creek",     15.5, 280.0}
    };
}

void lariIteratif(Kuda &k, double jarakFinish) {
    k.posisiSaatIni = 0;
    k.staminaSaatIni = k.staminaMaks;
    k.langkahDitempuh = 0;

    while (k.posisiSaatIni < jarakFinish) {
        k.langkahDitempuh++; 
        double kecepatanSaatIni;
        
        if (k.staminaSaatIni > 0) {
            kecepatanSaatIni = k.kecepatanDasar;
        } else {
            kecepatanSaatIni = k.kecepatanDasar * 0.20; 
        }

        k.posisiSaatIni += kecepatanSaatIni;
        k.staminaSaatIni -= (k.kecepatanDasar * 0.05);
    }
}

void prosesLariRekursif(Kuda &k, double jarakFinish) {
    if (k.posisiSaatIni >= jarakFinish) {
        return;
    }

    k.langkahDitempuh++;

    double kecepatanSaatIni;
    if (k.staminaSaatIni > 0) {
        kecepatanSaatIni = k.kecepatanDasar;
    } else {
        kecepatanSaatIni = k.kecepatanDasar * 0.20;
    }

    k.posisiSaatIni += kecepatanSaatIni;
    k.staminaSaatIni -= (k.kecepatanDasar * 0.05);

    prosesLariRekursif(k, jarakFinish);
}

void lariRekursif(Kuda &k, double jarakFinish) {
    k.posisiSaatIni = 0;
    k.staminaSaatIni = k.staminaMaks;
    k.langkahDitempuh = 0;
    prosesLariRekursif(k, jarakFinish);
}

bool bandingkanPemenang(const Kuda &a, const Kuda &b) {
    return a.langkahDitempuh < b.langkahDitempuh;
}

int main() {
    vector<RiwayatAnalisis> riwayat;
    int pilihanMenu;
    const int PENGULANGAN_BENCHMARK = 100;

    do {
        cout << "\n========================================" << endl;
        cout << "      MENU SIMULASI PACUAN KUDA" << endl;
        cout << "========================================" << endl;
        cout << "1. Lihat Daftar Kuda" << endl;
        cout << "2. Mulai Simulasi (Balapan)" << endl;
        cout << "3. Lihat Tabel Analisis Waktu" << endl;
        cout << "4. Keluar" << endl;
        cout << "========================================" << endl;
        cout << "Pilih menu : ";
        cin >> pilihanMenu;

        switch(pilihanMenu) {
            case 1: {
                vector<Kuda> daftar = dapatkanDataKuda();
                cout << "\n[DAFTAR KUDA]" << endl;
                cout << left << setw(5) << "No" << setw(20) << "Nama" << setw(12) << "Kecepatan" << "Stamina" << endl;
                cout << "--------------------------------------------------" << endl;
                for(const auto& k : daftar) {
                    cout << left << setw(5) << k.id 
                         << setw(20) << k.nama 
                         << setw(12) << k.kecepatanDasar 
                         << k.staminaMaks << endl;
                }
                break;
            }

            case 2: {
                double jarak;
                do {
                    cout << "\nMasukkan Jarak Tempuh : ";
                    cin >> jarak;
                    if(jarak < 0) cout << "[!] Jarak harus lebih dari 0!" << endl;
                } while(jarak < 0);

                cout << "\n>>> SEDANG BALAPAN (" << jarak << "m)..." << endl;

                vector<Kuda> peserta = dapatkanDataKuda();
                
                for(int i=0; i<16; i++) {
                    lariIteratif(peserta[i], jarak);
                }
                
                sort(peserta.begin(), peserta.end(), bandingkanPemenang);

                cout << "\n=== HASIL AKHIR BALAPAN ===" << endl;
                for(int i=0; i<16; i++) {
                    cout << "Juara " << setw(2) << i+1 << " : " 
                         << left << setw(20) << peserta[i].nama 
                         << " (" << peserta[i].langkahDitempuh << " langkah)" << endl;
                }

                Kuda penguji = {99, "Penguji", 18.0, 100.0};

                auto t1 = high_resolution_clock::now();
                for(int i=0; i<PENGULANGAN_BENCHMARK; i++) {
                    Kuda k = penguji; 
                    lariRekursif(k, jarak);
                }
                auto t2 = high_resolution_clock::now();
                duration<double, milli> durasiRec = t2 - t1;

                auto t3 = high_resolution_clock::now();
                for(int i=0; i<PENGULANGAN_BENCHMARK; i++) {
                    Kuda k = penguji; 
                    lariIteratif(k, jarak);
                }
                auto t4 = high_resolution_clock::now();
                duration<double, milli> durasiIter = t4 - t3;

                riwayat.push_back({(int)jarak, durasiRec.count(), durasiIter.count()});
                
                cout << "\n[Info] Data analisis waktu untuk jarak " << jarak << "m telah disimpan ke Menu 3." << endl;
                break;
            }

            case 3: {
                if(riwayat.empty()) {
                    cout << "\n[!] Belum ada data simulasi. Silakan jalankan Menu 2 terlebih dahulu." << endl;
                } else {
                    cout << "\n[TABEL ANALISIS WAKTU EKSEKUSI]" << endl;
                    cout << "+=========+=====================+=====================+" << endl;
                    cout << "|    N    | Waktu Rekursif (ms) | Waktu Iteratif (ms) |" << endl;
                    cout << "+=========+=====================+=====================+" << endl;

                    for(const auto& log : riwayat) {
                        cout << "| " << setw(7) << log.jarak << " | "
                             << setw(19) << fixed << setprecision(6) << log.waktuRekursif << " | "
                             << setw(19) << fixed << setprecision(6) << log.waktuIteratif << " |" << endl;
                    }
                    cout << "+---------+---------------------+---------------------+" << endl;
                }
                break;
            }

            case 4:
                cout << "Keluar dari program..." << endl;
                break;

            default:
                cout << "[!] Pilihan tidak valid." << endl;
        }

    } while (pilihanMenu != 4);

    return 0;
}