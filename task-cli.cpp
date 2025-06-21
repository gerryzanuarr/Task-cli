#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

const string FILENAME = "task.json";

struct Task {
    int id;
    string deskripsi;
    string status;
    string createdAt;
    string updatedAt;
};

// --- Utilitas ---
string escapeJson(const string& input) {
    string result;
    for (char c : input) {
        if (c == '"') result += "\\\"";
        else if (c == '\\') result += "\\\\";
        else result += c;
    }
    return result;
}

string getCurrentTime() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buf[20];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ltm);
    return string(buf);
}

string toJson(const Task& t) {
    return "  {\"id\": " + to_string(t.id) +
           ", \"description\": \"" + escapeJson(t.deskripsi) +
           "\", \"status\": \"" + escapeJson(t.status) +
           "\", \"createdAt\": \"" + t.createdAt +
           "\", \"updatedAt\": \"" + t.updatedAt + "\"}";
}

string extractJsonString(const string& line, const string& key) {
    size_t keyPos = line.find("\"" + key + "\"");
    if (keyPos == string::npos) return "";
    size_t colonPos = line.find(":", keyPos);
    size_t firstQuote = line.find("\"", colonPos);
    size_t secondQuote = line.find("\"", firstQuote + 1);
    return line.substr(firstQuote + 1, secondQuote - firstQuote - 1);
}

int extractJsonInt(const string& line, const string& key) {
    size_t keyPos = line.find("\"" + key + "\"");
    if (keyPos == string::npos) return 0;
    size_t colonPos = line.find(":", keyPos);
    size_t comma = line.find(",", colonPos);
    string num = line.substr(colonPos + 1, comma - colonPos - 1);
    return stoi(num);
}

// --- File I/O ---
vector<Task> loadTasks() {
    vector<Task> tasks;
    ifstream file(FILENAME);
    string line;
    while (getline(file, line)) {
        if (line.find("{") != string::npos) {
            Task t;
            t.id = extractJsonInt(line, "id");
            t.deskripsi = extractJsonString(line, "description");
            t.status = extractJsonString(line, "status");
            t.createdAt = extractJsonString(line, "createdAt");
            t.updatedAt = extractJsonString(line, "updatedAt");
            tasks.push_back(t);
        }
    }
    return tasks;
}

//savetoJSON
void saveTasks(const vector<Task>& tasks) {
    ofstream file(FILENAME);
    file << "[\n";
    for (size_t i = 0; i < tasks.size(); ++i) {
        file << toJson(tasks[i]);
        if (i != tasks.size() - 1) file << ",";
        file << "\n";
    }
    file << "]";
    file.close();
}

//cls
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

//pause
void pause() {
    cout << "\nTekan Enter untuk kembali ke menu...";
    cin.ignore();
    cin.get();
}

// --- Menu Aksi ---
string add() {
    vector<Task> tasks = loadTasks();
    Task t;

    cout << "Masukkan task: ";
    getline(cin >> ws, t.deskripsi);

    t.id = tasks.empty() ? 1 : tasks.back().id + 1;
    t.status = "todo";
    t.createdAt = getCurrentTime();
    t.updatedAt = t.createdAt;

    tasks.push_back(t);
    saveTasks(tasks);

    cout << "Task berhasil ditambahkan.\n";
    return t.deskripsi;
}

void update() {
    vector<Task> tasks = loadTasks();
    if (tasks.empty()) {
        cout << "Tidak ada task.\n";
        return;
    }

    for (const Task& t : tasks) {
        cout << t.id << ". " << t.deskripsi << " [" << t.status << "]\n";
    }

    int id;
    cout << "Pilih ID task yang ingin diubah: ";
    cin >> id;
    cin.ignore();

    for (Task& t : tasks) {
        if (t.id == id) {
            int statusPilihan;
			cout << "Status baru:\n";
			cout << "1. todo\n";
			cout << "2. in-progress\n";
			cout << "3. done\n";
			cout << "Pilih: ";
			cin >> statusPilihan;
			cin.ignore();
			
			switch (statusPilihan) {
			    case 1: t.status = "todo"; break;
			    case 2: t.status = "in-progress"; break;
			    case 3: t.status = "done"; break;
			    default:
			        cout << "Pilihan tidak valid. Status tidak diubah.\n";
			        return;
				}
            t.updatedAt = getCurrentTime();
            saveTasks(tasks);
            cout << "Status diperbarui.\n";
            return;
        }
    }
    cout << "ID tidak ditemukan.\n";
}

void del() {
    vector<Task> tasks = loadTasks();
    if (tasks.empty()) {
        cout << "Tidak ada task.\n";
        return;
    }

    for (const Task& t : tasks) {
        cout << t.id << ". " << t.deskripsi << " [" << t.status << "]\n";
    }

    int id;
    cout << "Pilih ID task yang ingin dihapus: ";
    cin >> id;
    cin.ignore();

    for (size_t i = 0; i < tasks.size(); ++i) {
        if (tasks[i].id == id) {
            tasks.erase(tasks.begin() + i);
            saveTasks(tasks);
            cout << "Task dihapus.\n";
            return;
        }
    }
    cout << "ID tidak ditemukan.\n";
}

void list() {
    vector<Task> tasks = loadTasks();
    if (tasks.empty()) {
        cout << "Tidak ada task.\n";
        return;
    }

    for (const Task& t : tasks) {
        cout << t.id << ". " << t.deskripsi << " [" << t.status << "]\n";
        cout << "   Dibuat: " << t.createdAt << "\n";
        cout << "   Diperbarui: " << t.updatedAt << "\n";
    }
}

void listByStatus(const string& filter) {
    vector<Task> tasks = loadTasks();
    bool found = false;
    for (const Task& t : tasks) {
        if (t.status == filter) {
            cout << t.id << ". " << t.deskripsi << endl;
            found = true;
        }
    }
    if (!found) {
        cout << "Tidak ada task dengan status \"" << filter << "\".\n";
    }
}

// --- Main ---
int main(int argc, char *argv[]) {
    int pilihan;
    do {
        clearScreen();
        cout << "--- Menu ---\n";
        cout << "1. Tambah task\n";
        cout << "2. Update status task\n";
        cout << "3. Hapus task\n";
        cout << "4. Lihat semua task\n";
        cout << "5. Lihat task selesai (done)\n";
        cout << "6. Lihat task belum dikerjakan (todo)\n";
        cout << "7. Lihat task dalam progress (in-progress)\n";
        cout << "0. Keluar\n";
        cout << "Pilihan: ";
        cin >> pilihan;
        cin.ignore();

        switch (pilihan) {
            case 1: clearScreen(); add(); pause(); break;
            case 2: clearScreen(); update(); pause(); break;
            case 3: clearScreen(); del(); pause(); break;
            case 4: clearScreen(); list(); pause(); break;
            case 5: clearScreen(); listByStatus("done"); pause(); break;
            case 6: clearScreen(); listByStatus("todo"); pause(); break;
            case 7: clearScreen(); listByStatus("in-progress"); pause(); break;
            case 0: clearScreen(); cout << "Keluar...\n"; break;
            default: cout << "Pilihan tidak valid.\n"; pause(); break;
        }
    } while (pilihan != 0);

    return 0;
}