#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <map>

using namespace std;

// Base Class
class Asset {
protected:
    string name;
    int size;

public:
    Asset(string name, int size) : name(name), size(size) {}
    virtual ~Asset() {} // Virtual destructor for safety
    virtual string getType() = 0;
    string getName() { return name; }
    int getSize() { return size; }
};

// Derived Classes
class Texture : public Asset {
public:
    Texture(string name, int size) : Asset(name, size) {}
    string getType() override { return "Texture"; }
};

class Sound : public Asset {
public:
    Sound(string name, int size) : Asset(name, size) {}
    string getType() override { return "Sound"; }
};

class Model : public Asset {
public:
    Model(string name, int size) : Asset(name, size) {}
    string getType() override { return "Model"; }
};

// LRU Cache
class LRUCache {
    int capacity;
    list<string> cache;

public:
    LRUCache(int cap) : capacity(cap) {}

    void useAsset(string name) {
        cache.remove(name);
        cache.push_front(name);
        if (cache.size() > capacity) {
            cache.pop_back();
        }
    }

    list<string> getCache() { return cache; }
};

// Duplicate Check
bool isDuplicate(const vector<Asset*>& assets, string name) {
    for (auto a : assets) {
        if (a->getName() == name) return true;
    }
    return false;
}

// Generate Dashboard
void generateHTML(const vector<Asset*>& assets, LRUCache& cache) {
    ofstream file("dashboard.html");

    // Memory distribution for Chart.js
    map<string, int> memDist;
    memDist["Texture"] = 0;
    memDist["Sound"] = 0;
    memDist["Model"] = 0;
    int totalMem = 0;

    for (auto a : assets) {
        memDist[a->getType()] += a->getSize();
        totalMem += a->getSize();
    }

    file << "<!DOCTYPE html><html><head><title>Asset Manager Pro</title>";
    file << "<script src='https://cdn.jsdelivr.net/npm/chart.js'></script>";
    file << "<link href='https://fonts.googleapis.com/css2?family=Segoe+UI:wght@300;400;600&display=swap' rel='stylesheet'>";
    file << "<style>";
    file << "body{ font-family: 'Segoe UI', sans-serif; background: #0b0e14; color: white; margin: 0; padding: 20px; }";
    file << ".container{ max-width: 1100px; margin: auto; display: flex; flex-wrap: wrap; gap: 20px; }";
    file << ".main-panel{ flex: 2; min-width: 600px; }";
    file << ".side-panel{ flex: 1; min-width: 300px; background: #161b22; padding: 20px; border-radius: 15px; height: fit-content; }";
    
    // Header & Search
    file << ".header{ width: 100%; display: flex; justify-content: space-between; align-items: center; margin-bottom: 30px; }";
    file << "input[type='text']{ background: #21262d; border: 1px solid #30363d; color: white; padding: 10px 20px; border-radius: 20px; width: 300px; outline: none; }";
    file << "input[type='text']:focus{ border-color: #58a6ff; }";

    // Cards
    file << ".grid{ display: grid; grid-template-columns: repeat(auto-fill, minmax(180px, 1fr)); gap: 15px; }";
    file << ".card{ background: #161b22; border: 1px solid #30363d; padding: 15px; border-radius: 10px; transition: 0.2s; }";
    file << ".card:hover{ border-color: #58a6ff; transform: translateY(-3px); }";
    file << ".Texture{ border-left: 4px solid #238636; } .Sound{ border-left: 4px solid #1f6feb; } .Model{ border-left: 4px solid #8957e5; }";
    file << "h3{ margin: 5px 0; font-size: 0.95rem; word-break: break-all; }";
    file << ".size-tag{ color: #8b949e; font-size: 0.8rem; }";
    
    file << "</style></head><body>";

    file << "<div class='container'>";
    file << "<div class='header'><h1>Asset Console <small style='color:#58a6ff; font-size:14px;'>v2.0</small></h1>";
    file << "<input type='text' id='searchInput' onkeyup='filterAssets()' placeholder='Search assets...'></div>";

    // Main Content
    file << "<div class='main-panel'>";
    file << "<h2>Active Assets</h2><div class='grid' id='assetGrid'>";
    for (auto a : assets) {
        file << "<div class='card " << a->getType() << "'>";
        file << "<small style='color:#8b949e; text-transform:uppercase; font-size:10px;'>" << a->getType() << "</small>";
        file << "<h3>" << a->getName() << "</h3>";
        file << "<span class='size-tag'>" << a->getSize() << " MB</span>";
        file << "</div>";
    }
    file << "</div></div>";

    // Side Content (Chart & LRU)
    file << "<div class='side-panel'>";
    file << "<h3>Memory Usage</h3>";
    file << "<canvas id='memChart' width='200' height='200'></canvas>";
    file << "<h4 style='text-align:center; margin-top:10px;'>Total: " << totalMem << " MB</h4>";
    
    file << "<hr style='border:0; border-top:1px solid #30363d; margin: 20px 0;'>";
    file << "<h3>Recent Activity</h3><ul style='list-style:none; padding:0; font-size:13px; color:#c9d1d9;'>";
    for (auto const& name : cache.getCache()) {
        file << "<li style='padding:5px 0; border-bottom: 1px solid #21262d;'> > " << name << "</li>";
    }
    file << "</ul></div>";

    file << "</div>"; // Close container

    // JavaScript for Search and Chart
    file << "<script>";
    // Search Function
    file << "function filterAssets() {";
    file << "  let input = document.getElementById('searchInput').value.toLowerCase();";
    file << "  let cards = document.getElementsByClassName('card');";
    file << "  for(let i=0; i<cards.size; i++){}"; // Dummy logic placeholder
    file << "  Array.from(cards).forEach(card => {";
    file << "    let name = card.getElementsByTagName('h3')[0].innerText.toLowerCase();";
    file << "    card.style.display = name.includes(input) ? '' : 'none';";
    file << "  });";
    file << "}";

    // Chart.js Data
    file << "const ctx = document.getElementById('memChart').getContext('2d');";
    file << "new Chart(ctx, { type: 'pie', data: {";
    file << "  labels: ['Textures', 'Sounds', 'Models'],";
    file << "  datasets: [{";
    file << "    data: [" << memDist["Texture"] << "," << memDist["Sound"] << "," << memDist["Model"] << "],";
    file << "    backgroundColor: ['#238636', '#1f6feb', '#8957e5'], borderWeight: 0";
    file << "  }]";
    file << "}, options: { plugins: { legend: { labels: { color: 'white' } } } } });";
    file << "</script>";

    file << "</body></html>";
    file.close();
}

int main() {
    vector<Asset*> assets;
    LRUCache cache(5);

    // Mock Loading (In a real project, read these from a file!)
    string names[] = {"player_diffuse.png", "bg_music.wav", "boss_model.obj", "explosion.wav", "ui_icons.png", "tree_high_poly.obj"};
    string types[] = {"tex", "snd", "mod", "snd", "tex", "mod"};
    int sizes[] = {45, 12, 120, 5, 10, 85};

    for(int i=0; i<6; i++) {
        if(!isDuplicate(assets, names[i])) {
            if(types[i] == "tex") assets.push_back(new Texture(names[i], sizes[i]));
            else if(types[i] == "snd") assets.push_back(new Sound(names[i], sizes[i]));
            else assets.push_back(new Model(names[i], sizes[i]));
            cache.useAsset(names[i]);
        }
    }

    generateHTML(assets, cache);
    cout << "Advanced Dashboard Generated: Open dashboard.html to view.\n";

    // Clean up
    for(auto a : assets) delete a;
    return 0;
}
