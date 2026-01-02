#include <iostream>
#include <vector>
#include <string>
#include <chrono>   
#include <iomanip> 
#include <algorithm> 
#include <fstream>  
#include <cstdlib>

using namespace std;
using namespace std::chrono;

// --- STRUKTUR DATA ---
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

// --- DATA SOURCE ---
vector<Kuda> dapatkanDataKuda() {
    return {
        {1, "Symboli Kris S", 19.5, 160.0}, {2, "Mejiro Ryan", 18.0, 180.0}, 
        {3, "Narita Brian", 21.0, 130.0}, {4, "Winning Ticket", 17.5, 190.0}, 
        {5, "Symboli Rudolf", 20.0, 155.0}, {6, "Goldship", 17.0, 250.0}, 
        {7, "Gold City", 18.2, 165.0}, {8, "Fenomeno", 17.8, 175.0},
        {9, "Tokai Teio", 19.8, 140.0}, {10, "Special Week", 18.5, 170.0},
        {11, "Still In Love", 16.5, 200.0}, {12, "Oguri Cap", 19.0, 160.0},
        {13, "Mahantan Caffe", 16.0, 240.0}, {14, "Agnes Tachyon", 19.2, 145.0},
        {15, "Jungle Pocket", 18.8, 150.0}, {16, "Super Creek", 15.5, 280.0}
    };
}

// --- LOGIKA ALGORITMA ---
void lariIteratif(Kuda &k, double jarakFinish) {
    k.posisiSaatIni = 0; k.staminaSaatIni = k.staminaMaks; k.langkahDitempuh = 0;
    while (k.posisiSaatIni < jarakFinish) {
        k.langkahDitempuh++; 
        double vel = (k.staminaSaatIni > 0) ? k.kecepatanDasar : k.kecepatanDasar * 0.20;
        k.posisiSaatIni += vel; k.staminaSaatIni -= (k.kecepatanDasar * 0.05);
    }
}

void prosesLariRekursif(Kuda &k, double jarakFinish) {
    if (k.posisiSaatIni >= jarakFinish) return;
    k.langkahDitempuh++;
    double vel = (k.staminaSaatIni > 0) ? k.kecepatanDasar : k.kecepatanDasar * 0.20;
    k.posisiSaatIni += vel; k.staminaSaatIni -= (k.kecepatanDasar * 0.05);
    prosesLariRekursif(k, jarakFinish);
}

void lariRekursif(Kuda &k, double jarakFinish) {
    k.posisiSaatIni = 0; k.staminaSaatIni = k.staminaMaks; k.langkahDitempuh = 0;
    prosesLariRekursif(k, jarakFinish);
}

// --- FUNGSI EXPORT HTML (TAMPILAN TETAP SAMA) ---
void exportKeHTML(vector<RiwayatAnalisis> data) {
    if(data.empty()) {
        cout << "\n[!] Data kosong. Lakukan simulasi (Menu 2) dulu!" << endl;
        return;
    }

    sort(data.begin(), data.end(), [](const RiwayatAnalisis& a, const RiwayatAnalisis& b) {
        return a.jarak < b.jarak;
    });

    string namaFile = "Hasil_Grafik_Kuda.html";
    ofstream fileHTML(namaFile); 

    if (fileHTML.is_open()) {
        fileHTML << "<html>\n<head>\n";
        fileHTML << "<title>Analisis Performa</title>\n";
        fileHTML << "<script type='text/javascript' src='https://www.gstatic.com/charts/loader.js'></script>\n";
        fileHTML << "<script type='text/javascript'>\n";
        fileHTML << "  google.charts.load('current', {'packages':['corechart']});\n";
        fileHTML << "  google.charts.setOnLoadCallback(drawChart);\n";
        fileHTML << "  function drawChart() {\n";
        fileHTML << "    var data = google.visualization.arrayToDataTable([\n";
        fileHTML << "      ['Jarak (N)', 'Waktu Rekursif (ms)', 'Waktu Iteratif (ms)'],\n";

        for (const auto& r : data) {
            fileHTML << "      [" << r.jarak << ", " << r.waktuRekursif << ", " << r.waktuIteratif << "],\n";
        }

        fileHTML << "    ]);\n";
        fileHTML << "    var options = {\n";
        fileHTML << "      curveType: 'function',\n"; 
        fileHTML << "      legend: { position: 'bottom' },\n";
        fileHTML << "      pointSize: 7,\n"; 
        fileHTML << "      hAxis: {title: 'Jarak Tempuh (m) atau N'},\n";
        fileHTML << "      vAxis: {title: 'Waktu Eksekusi (ms)'},\n";
        fileHTML << "      colors: ['#4285F4', '#DB4437']\n"; 
        fileHTML << "    };\n";
        fileHTML << "    var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));\n";
        fileHTML << "    chart.draw(data, options);\n";
        fileHTML << "  }\n";
        fileHTML << "</script>\n</head>\n<body>\n";
        fileHTML << "<h2 style='font-family: Arial; text-align: center;'>Diagram Analisis Waktu Eksekusi Komputer Dalam Perhitungan Pacuan Kuda</h2>\n";
        fileHTML << "<div id='curve_chart' style='width: 900px; height: 500px; margin: 0 auto;'></div>\n";
        fileHTML << "<table border='1' style='margin: 20px auto; border-collapse: collapse; font-family: Arial;'>\n";
        fileHTML << "<tr style='background-color: #f2f2f2;'><th>Jarak (m)</th><th>Rekursif (ms)</th><th>Iteratif (ms)</th></tr>\n";
        for (const auto& r : data) {
             fileHTML << "<tr><td style='padding:8px; text-align:center;'>" << r.jarak << "</td>";
             fileHTML << "<td style='padding:8px; text-align:center;'>" << r.waktuRekursif << "</td>";
             fileHTML << "<td style='padding:8px; text-align:center;'>" << r.waktuIteratif << "</td></tr>\n";
        }
        fileHTML << "</table>\n";
        fileHTML << "</body>\n</html>";
        fileHTML.close();
        
        cout << "\n==============================================" << endl;
        cout << " [SUKSES] File Grafik berhasil dibuat!" << endl;
        cout << " Nama File : " << namaFile << endl;
        cout << "==============================================" << endl;
    } else {
        cout << "\n[!] Gagal membuat file." << endl;
    }
}

// --- MAIN PROGRAM ---
int main() {
    vector<RiwayatAnalisis> riwayat;
    int pilihanMenu;
    const int PENGULANGAN_BENCHMARK = 100;

    do {
        cout << "\n========================================" << endl;
        cout << "      MENU SIMULASI PACUAN KUDA" << endl;
        cout << "========================================" << endl;
        cout << "1. Lihat Daftar Kuda" << endl;
        cout << "2. Mulai Simulasi & Benchmarking" << endl;
        cout << "3. Lihat Tabel Data (Console)" << endl;
        cout << "4. Generate Grafik (.HTML)" << endl;
        cout << "5. Edit Nilai N" << endl;
        cout << "6. Hapus Salah Satu Perhitungan" << endl;
        cout << "7. Keluar" << endl;
        cout << "========================================" << endl;
        cout << "Pilih menu : ";
        cin >> pilihanMenu;

        switch(pilihanMenu) {
            case 1: {
                vector<Kuda> daftar = dapatkanDataKuda();
                cout << "\n[DAFTAR KUDA]" << endl;
                cout << "No   Nama                  Kecepatan" << endl;
                cout << "-----------------------------------" << endl;
                for(const auto& k : daftar) {
                    cout << left << setw(5) << k.id << setw(21) << k.nama << k.kecepatanDasar << endl;
                }
                break;
            }
            case 2: {
                double jarak;
                do {
                    cout << "\nMasukkan Jarak Tempuh : ";
                    cin >> jarak;
                    if(jarak < 0) cout << "[!] Jarak harus > 0!" << endl;
                } while(jarak < 0);

                cout << "\n>>> SEDANG BALAPAN (" << jarak << "m)..." << endl;
                Kuda penguji = {99, "Penguji", 18.0, 100.0};
                
                auto t1 = high_resolution_clock::now();
                for(int i=0; i<PENGULANGAN_BENCHMARK; i++) { Kuda k = penguji; lariRekursif(k, jarak); }
                auto t2 = high_resolution_clock::now();
                
                auto t3 = high_resolution_clock::now();
                for(int i=0; i<PENGULANGAN_BENCHMARK; i++) { Kuda k = penguji; lariIteratif(k, jarak); }
                auto t4 = high_resolution_clock::now();

                duration<double, milli> durasiRec = t2 - t1;
                duration<double, milli> durasiIter = t4 - t3;

                riwayat.push_back({(int)jarak, durasiRec.count(), durasiIter.count()});
                cout << "[Info] Data tersimpan." << endl;
                break;
            }
            case 3: {
                if(riwayat.empty()) {
                    cout << "\n[!] Data kosong." << endl;
                } else {
                    cout << "\n[TABEL DATA]" << endl;
                    cout << "   N    |  Rekursif (ms) |  Iteratif (ms)" << endl;
                    cout << "--------+----------------+----------------" << endl;
                    for(const auto& r : riwayat) 
                        cout << setw(7) << r.jarak << " | " << setw(14) << fixed << setprecision(4) << r.waktuRekursif << " | " << r.waktuIteratif << endl;
                }
                break;
            }
            case 4: exportKeHTML(riwayat); break;

            case 5: { // EDIT NILAI N
                if(riwayat.empty()) { cout << "[!] Data kosong."; break; }
                int targetN, baruN;
                cout << "Masukkan nilai N yang ingin diubah: "; cin >> targetN;
                bool ketemu = false;
                for(auto& r : riwayat) {
                    if(r.jarak == targetN) {
                        cout << "Masukkan nilai N baru: "; cin >> baruN;
                        // Hitung ulang benchmark untuk N baru
                        Kuda penguji = {99, "Penguji", 18.0, 100.0};
                        auto t1 = high_resolution_clock::now();
                        for(int i=0; i<PENGULANGAN_BENCHMARK; i++) { Kuda k = penguji; lariRekursif(k, (double)baruN); }
                        auto t2 = high_resolution_clock::now();
                        auto t3 = high_resolution_clock::now();
                        for(int i=0; i<PENGULANGAN_BENCHMARK; i++) { Kuda k = penguji; lariIteratif(k, (double)baruN); }
                        auto t4 = high_resolution_clock::now();

                        r.jarak = baruN;
                        r.waktuRekursif = duration<double, milli>(t2 - t1).count();
                        r.waktuIteratif = duration<double, milli>(t4 - t3).count();
                        ketemu = true;
                        cout << "[!] Berhasil diubah dan dihitung ulang." << endl;
                        break;
                    }
                }
                if(!ketemu) cout << "[!] Nilai N tidak ditemukan." << endl;
                break;
            }

            case 6: { // HAPUS SALAH SATU
                if(riwayat.empty()) { cout << "[!] Data kosong."; break; }
                int hapusN;
                cout << "Masukkan nilai N yang ingin dihapus: "; cin >> hapusN;
                auto it = remove_if(riwayat.begin(), riwayat.end(), [hapusN](RiwayatAnalisis r){ return r.jarak == hapusN; });
                if(it != riwayat.end()) {
                    riwayat.erase(it, riwayat.end());
                    cout << "[!] Data N = " << hapusN << " berhasil dihapus." << endl;
                } else cout << "[!] Data tidak ditemukan." << endl;
                break;
            }

            case 7: cout << "Keluar..." << endl; break;
            default: cout << "[!] Pilihan salah." << endl;
        }
    } while (pilihanMenu != 7);
    return 0;
}