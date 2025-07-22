# Tubes1_CaKrai17

1. Bagian awal kode mengimpor library standar C++ yang dibutuhkan:
* `#include <iostream>` untuk mencetak ke layar.
* `#include <chrono>` dan `#include <thread>` untuk mengatur waktu dan delay.
* `#include <vector>` dan `#include <string>` untuk menyimpan dan mengelola data.
* `#include <cstdint>` untuk menggunakan tipe data seperti `uint32_t`.
Selain itu, fungsi global `millis()` digunakan untuk menghitung waktu (dalam milidetik) sejak program dijalankan. Ini berguna untuk mencatat waktu transisi antar state.

2. Konstruktor dan Destruktor FSM
FSM adalah model komputasi yang menggambarkan perilaku atau prinsip kerja suatu sistem dengan sejumlah keadaan (state), kejadian (event), dan aksi (action) yang terbatas.

Konstruktor adalah fungsi khusus dalam kelas yang dipanggil otomatis saat objek dibuat

Destruktor adalah fungsi khusus dalam kelas yang dipanggil otomatis saat objek dihapus

* Konstruktor `FSM::FSM()` menginisialisasi FSM ke **state awal (`INIT`)** dengan nilai awal default (delay 0, error 0, move 0).
* Konstruktor `FSM::FSM(uint32_t delay)` mengatur nilai delay secara langsung saat objek FSM dibuat.
* Destruktor `~FSM()` membersihkan `stateHistory` saat objek dihapus, untuk mencegah memory leak.

3. Getter dan Setter
Getter adalah fungsi untuk mengambil nilai atribut dari kelas sedangkan Setter adalah fungsi untuk mengatur nilai atribut dalam kelas.

Fungsi-fungsi ini bertugas **mengakses (getter)** dan **mengubah (setter)** nilai-nilai internal FSM:

* `setDelay()` dan `getDelay()` untuk mengatur dan membaca nilai delay.
* `setErrorCount()` dan `getErrorCount()` untuk jumlah error.
* `setMoveCount()` dan `getMoveCount()` untuk jumlah gerakan.
* `setLastHeartbeat()` dan `getLastHeartbeat()` untuk mencatat waktu update terakhir.
* `getCurrentState()` dan `transitionToState()` untuk membaca dan berpindah antar state.
* `addStateToHistory()` dan `getStateHistory()` menyimpan jejak transisi state.

4. Fungsi `start()`
Ini adalah **fungsi utama untuk menjalankan FSM**. Selama state belum `STOPPED`, FSM akan terus memanggil `update()` setiap 1 detik menggunakan `std::this_thread::sleep_for`.

5. Fungsi `update()`
Fungsi ini mengecek state aktif saat ini, lalu memanggil fungsi yang sesuai:

* Jika di `INIT`, maka jalankan `performInit()`.
* Jika di `IDLE`, jalankan `performProcess()` (untuk menerima input user).
* Dan seterusnya untuk MOVEMENT, SHOOTING, CALCULATION, ERROR, STOPPED.

Setelah aksi dilakukan, waktu `lastHeartbeat` dan histori state diperbarui.

6. Status dan Riwayat
* `printStatus()` mencetak informasi lengkap tentang FSM saat ini: nama state, waktu heartbeat, delay, error, dan move count.
* `printStateHistory()` mencetak seluruh jejak transisi FSM beserta waktunya. Ini sangat berguna untuk melihat alur kerja FSM.


7. Fungsi State (Aksi Tiap State)
Fungsi-fungsi berikut menangani apa yang dilakukan FSM saat berada di state tertentu:

* **`performInit()`**
  Inisialisasi awal: mengatur delay dan langsung berpindah ke `IDLE`.

* **`performProcess()`**
  Menampilkan menu pilihan ke user: bisa lihat status, bergerak, menembak, atau menghitung. Input dari user menentukan transisi state berikutnya.

* **`performMovement()`**
  FSM akan mencetak "Moving...", menambah move count. Jika sudah 3 kali bergerak, otomatis lanjut ke `SHOOTING`, jika belum kembali ke `IDLE`.

* **`performShooting()`**
  FSM menembak dan mengatur ulang move count, kemudian kembali ke `IDLE`.

* **`performCalculation()`**
  Jika move count = 0, maka FSM masuk ke state `ERROR`. Kalau tidak, lanjut ke `IDLE`.

* **`performErrorHandling()`**
  Jika error terjadi, error count bertambah. Jika lebih dari 3 error, FSM berhenti (`STOPPED`). Kalau belum, balik ke `IDLE`.

* **`shutdown()`**
  Menandakan FSM berhenti sepenuhnya. Histori state akan dihapus.

