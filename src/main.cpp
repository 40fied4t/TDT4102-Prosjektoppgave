#include "tests/include/testGraph.h"
#include "include/graph.h"
#include "include/graphWindow.h"

int main(int argc, char* argv[]) {
    GraphWindow window;
    switch (argc) {
        case 1:
            window = GraphWindow();
            break;
        case 2:
            window = GraphWindow(std::filesystem::path{argv[1]});
            break;
        default:
            std::cerr << "Too many arguments";
            return 1;
    }
    window.run();
    return 0;
}