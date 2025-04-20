#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include "DayList.h"
#include "HeapSort.h"

// Checkbox UI element
typedef sf::Vector2f Vec2;
struct Checkbox {
    sf::RectangleShape box;
    sf::Text label;
    bool checked = false;

    Checkbox(const sf::Font &font, const std::string &text, Vec2 pos) {
        box.setSize({20,20});
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);
        box.setPosition(pos);

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::Black);
        label.setPosition(pos + Vec2(30.f, -4.f));
    }

    void draw(sf::RenderWindow &win) {
        win.draw(box);
        if (checked) {
            sf::RectangleShape tick({14,14});
            tick.setFillColor(sf::Color::Black);
            tick.setPosition(box.getPosition() + Vec2(3.f,3.f));
            win.draw(tick);
        }
        win.draw(label);
    }

    bool contains(const Vec2 &pt) const {
        return box.getGlobalBounds().contains(pt);
    }
    void toggle() { checked = !checked; }
};

// Button UI element
struct Button {
    sf::RectangleShape rect;
    sf::Text label;
    std::function<void()> callback;

    Button(const sf::Font &font, const std::string &text, Vec2 pos, std::function<void()> cb)
    : callback(cb) {
        rect.setSize({100,40});
        rect.setFillColor(sf::Color(200,200,200));
        rect.setOutlineColor(sf::Color::Black);
        rect.setOutlineThickness(2);
        rect.setPosition(pos);

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(18);
        label.setFillColor(sf::Color::Black);
        auto lb = label.getLocalBounds();
        label.setPosition(pos + Vec2((rect.getSize().x - lb.width)/2 - lb.left,
                                     (rect.getSize().y - lb.height)/2 - lb.top));
    }

    void draw(sf::RenderWindow &win) {
        win.draw(rect);
        win.draw(label);
    }
    bool contains(const Vec2 &pt) const {
        return rect.getGlobalBounds().contains(pt);
    }
};

// Text input UI element
struct TextInput {
    sf::RectangleShape box;
    sf::Text text;
    std::string content;
    bool active = false;

    TextInput(const sf::Font &font, Vec2 pos, Vec2 size) {
        box.setPosition(pos);
        box.setSize(size);
        box.setFillColor(sf::Color::White);
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);

        text.setFont(font);
        text.setCharacterSize(18);
        text.setFillColor(sf::Color::Black);
        text.setPosition(pos + Vec2(5.f,5.f));
    }

    void draw(sf::RenderWindow &win) {
        win.draw(box);
        win.draw(text);
    }
    bool contains(const Vec2 &pt) const {
        return box.getGlobalBounds().contains(pt);
    }
    void addChar(sf::Uint32 c) {
        if (c == '\b' && !content.empty()) content.pop_back();
        else if (c < 128) content += static_cast<char>(c);
        text.setString(content);
    }
};

int main() {
    // Create window
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Theme Park Crowd Calendar");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("assets/arial.ttf")) return -1;

    // Title
    sf::Text title("Theme Park Crowd Calendar", font, 32);
    title.setPosition({20.f,20.f});
    title.setFillColor(sf::Color::Black);

    // Park input
    TextInput parkInput(font, {600.f,110.f}, {350.f,30.f});
    sf::Text parkLabel("Theme Park(s):", font, 20);
    parkLabel.setPosition({600.f,80.f});
    parkLabel.setFillColor(sf::Color::Black);

    // Priority checkboxes
    std::vector<Checkbox> priorityBox;
    std::vector<std::string> priors = {"Crowd Size","Weather","Both","Don't Care"};
    for (int i=0; i<priors.size(); ++i)
        priorityBox.emplace_back(font, priors[i], Vec2(600.f,160.f + i*40.f));

    // Algorithm checkboxes
    Checkbox heapSortBox(font, "Heap Sort", {50.f,650.f});
    Checkbox quickSortBox(font, "Quick Sort", {200.f,650.f});

    // Search button
    Button searchBtn(font, "SEARCH", {450.f,650.f}, [&](){

    });

    // Column headers
    std::vector<sf::Text> colText;
    std::vector<std::string> columns = {"Park","Month","Day","Before/After","Capacity","Weather"};
    for (int i=0; i<columns.size(); ++i) {
        sf::Text t(columns[i], font, 20);
        t.setPosition({50.f + i*150.f, 300.f});
        t.setFillColor(sf::Color::Black);
        colText.push_back(t);
    }

    // Main loop
    while (window.isOpen()) {
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();
            else if (ev.type == sf::Event::MouseButtonPressed) {
                Vec2 mpos = window.mapPixelToCoords({ev.mouseButton.x, ev.mouseButton.y});
                for (auto &cb: priorityBox) if (cb.contains(mpos)) cb.toggle();
                if (heapSortBox.contains(mpos)) heapSortBox.toggle();
                if (quickSortBox.contains(mpos)) quickSortBox.toggle();
                if (searchBtn.contains(mpos)) searchBtn.callback();
                parkInput.active = parkInput.contains(mpos);
            } else if (ev.type == sf::Event::TextEntered && parkInput.active) {
                parkInput.addChar(ev.text.unicode);
            }
        }

        window.clear({240,240,240});
        window.draw(title);
        window.draw(parkLabel);
        parkInput.draw(window);
        for (auto &cb: priorityBox) cb.draw(window);
        heapSortBox.draw(window);
        quickSortBox.draw(window);
        searchBtn.draw(window);
        for (auto &ct: colText) window.draw(ct);
        window.display();
    }
    return 0;
}
