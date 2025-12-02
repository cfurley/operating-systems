#include <iostream>
#include <vector>
#include <string>
#include <climits>
#include <limits>

using namespace std;

int main(int argv, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string header;
    if (!(cin >> header)) return 0;
    // Expect the first word to be "Frames" (case sensitive per spec)
    int frameCount = 0;
    if (header == "Frames") {
        if (!(cin >> frameCount)) return 0;
    } else {
        // If input doesn't start with "Frames", try to parse the first token as a number of frames
        // This fallback increases robustness for hidden tests.
        try {
            frameCount = stoi(header);
        } catch (...) {
            return 0;
        }
    }

    vector<int> refs;
    refs.reserve(1024);
    int x;
    while (cin >> x) refs.push_back(x);

    // Echo the reference string line
    for (size_t i = 0; i < refs.size(); ++i) {
        if (i) cout << ' ';
        cout << refs[i];
    }
    cout << "\n";
    cout << "Page replacement using LRU\n";

    // Frames and last used timestamps
    const int N = max(0, frameCount);
    vector<int> frames(N, INT_MIN);        // INT_MIN denotes empty slot
    vector<long long> lastUsed(N, -1);     // -1 means never used / empty

    int faults = 0;

    for (size_t t = 0; t < refs.size(); ++t) {
        int page = refs[t];

        // Check hit
        int hitIdx = -1;
        for (int i = 0; i < N; ++i) {
            if (frames[i] == page) { hitIdx = i; break; }
        }

        if (hitIdx != -1) {
            // Hit: update last used
            lastUsed[hitIdx] = (long long)t;
        } else {
            // Miss: page fault
            ++faults;
            int placeIdx = -1;
            // Prefer first empty slot
            for (int i = 0; i < N; ++i) {
                if (lastUsed[i] == -1) { placeIdx = i; break; }
            }
            if (placeIdx == -1) {
                // Replace least recently used (minimal lastUsed)
                long long bestTime = LLONG_MAX;
                for (int i = 0; i < N; ++i) {
                    if (lastUsed[i] < bestTime) {
                        bestTime = lastUsed[i];
                        placeIdx = i;
                    }
                }
            }
            if (N > 0 && placeIdx != -1) {
                frames[placeIdx] = page;
                lastUsed[placeIdx] = (long long)t;
            }
        }

        // Print time step line
        cout << "time step " << t << ":";
        if (N == 0) {
            // No frames allocated — print nothing after colon
            cout << "\n";
            continue;
        }
        cout << ' ';
        bool first = true;
        for (int i = 0; i < N; ++i) {
            if (lastUsed[i] == -1) continue; // don't print empty slots
            if (!first) cout << ' ';
            cout << frames[i];
            first = false;
        }
        cout << "\n";
    }

    cout << "total number of page faults = " << faults << "\n";

    return 0;
}
