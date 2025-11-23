#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <input-file>\n";
        return 1;
    }

    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Error: could not open input file '" << argv[1] << "'.\n";
        return 1;
    }

    int n, m; // n = #processes, m = #resource types
    if (!(in >> n >> m) || n <= 0 || m <= 0) {
        std::cerr << "Error: invalid n or m in input file.\n";
        return 1;
    }

    std::vector<int> total(m);
    for (int j = 0; j < m; ++j) {
        if (!(in >> total[j]) || total[j] < 0) {
            std::cerr << "Error: invalid total resources in input file.\n";
            return 1;
        }
    }

    std::vector<std::vector<int>> allocation(n, std::vector<int>(m));
    std::vector<std::vector<int>> maxNeed(n, std::vector<int>(m));

    // Read Allocation matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (!(in >> allocation[i][j]) || allocation[i][j] < 0) {
                std::cerr << "Error: invalid allocation value in input file.\n";
                return 1;
            }
        }
    }

    // Read Max matrix
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (!(in >> maxNeed[i][j]) || maxNeed[i][j] < 0) {
                std::cerr << "Error: invalid max value in input file.\n";
                return 1;
            }
            if (maxNeed[i][j] < allocation[i][j]) {
                std::cerr << "Error: max < allocation for process "
                          << i << ", resource " << j << ".\n";
                return 1;
            }
        }
    }

    // Compute Available = total - sum(allocation)
    std::vector<int> available(m);
    for (int j = 0; j < m; ++j) {
        int sumAlloc = 0;
        for (int i = 0; i < n; ++i) {
            sumAlloc += allocation[i][j];
        }
        if (sumAlloc > total[j]) {
            std::cerr << "Error: allocated resources exceed total for resource "
                      << j << ".\n";
            return 1;
        }
        available[j] = total[j] - sumAlloc;
    }

    // Safety algorithm
    std::vector<int> work = available;
    std::vector<bool> finish(n, false);
    std::vector<int> safeSequence;
    safeSequence.reserve(n);

    int finishedCount = 0;
    bool progress = true;

    while (finishedCount < n && progress) {
        progress = false;

        for (int i = 0; i < n; ++i) {
            if (finish[i]) {
                continue;
            }

            bool canFinish = true;
            for (int j = 0; j < m; ++j) {
                int need_ij = maxNeed[i][j] - allocation[i][j];
                if (need_ij > work[j]) {
                    canFinish = false;
                    break;
                }
            }

            if (canFinish) {
                // Simulate P_i finishing: release its allocated resources
                for (int j = 0; j < m; ++j) {
                    work[j] += allocation[i][j];
                }
                finish[i] = true;
                ++finishedCount;
                safeSequence.push_back(i);
                progress = true;
            }
        }
    }

    if (finishedCount == n) {
        std::cout << "The system is in a SAFE state.\n";
        std::cout << "Safe sequence: ";

        for (std::size_t k = 0; k < safeSequence.size(); ++k) {
            std::cout << 'P' << safeSequence[k];
            if (k + 1 < safeSequence.size()) {
                std::cout << " -> ";
            }
        }
        std::cout << '\n';
    } else {
        std::cout << "The system is NOT in a safe state (unsafe).\n";
    }

    return 0;
}
