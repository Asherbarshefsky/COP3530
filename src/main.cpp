#include <SFML/Graphics.hpp>
#include "DayList.h"
#include <vector>
#include <string>

struct Checkbox {
    sf::RectangleShape box;
    sf::Text label;
    bool checked = false;

    Checkbox(const sf::Font& font, const std::string& text, float x, float y) : box(), label(font, sf::String(text), 16) {
        box.setSize({20, 20});
        box.setPosition({x, y});
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);
        box.setFillColor(sf::Color::White);

        label.setFillColor(sf::Color::Black);
        label.setPosition({x + 25, y - 2});
    }

    void draw(sf::RenderWindow& window) {
        box.setFillColor(checked ? sf::Color::Black : sf::Color::White);
        window.draw(box);
        window.draw(label);
    }

    bool toggleIfClicked(float mx, float my) {
        if (box.getGlobalBounds().contains({mx, my})) {
            checked = !checked;
            return true;
        }

        return false;
    }
};

struct Button {
    sf::RectangleShape box;
    sf::Text label;

    Button(const sf::Font& font, const std::string& text, float x, float y, float w, float h) : box(), label(font, sf::String(text), 18) {
        box.setSize({w, h});
        box.setPosition({x, y});
        box.setFillColor(sf::Color(200,200,200));
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);

        sf::FloatRect b = label.getLocalBounds();

        label.setPosition({
            x + (w - b.size.x)/2.f - b.position.x,
            y + (h - b.size.y)/2.f - b.position.y
        });

        label.setFillColor(sf::Color::Black);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(box);
        window.draw(label);
    }

    bool isClicked(float mx, float my) {
        return box.getGlobalBounds().contains({mx, my});
    }
};

// Display sorted days
struct Result {
    std::string park;
    unsigned int month
    unsigned int day;
    float capacity;
    unsigned int weather;
};

int main() {
    // Selection window
    sf::Vector2u selectSize(600u, 400u);
    sf::RenderWindow selectWin(sf::VideoMode(selectSize), sf::String("Select Priority"));

    sf::Font font;

    if (!font.openFromFile("src/arial.ttf"))
        return -1;

    // Priority checkboxes
    std::vector<Checkbox> priority = {
        Checkbox(font, "By Capacity", 20,  90),
        Checkbox(font, "By Weather" , 20, 130),
        Checkbox(font, "By Both"    , 20, 170)
    };

    priority[0].checked = true;

    // Algorithm checkboxes
    std::vector<Checkbox> algo = {
        Checkbox(font, "Heap Sort" , 20, 260),
        Checkbox(font, "Quick Sort", 20, 300)
    };

    algo[0].checked = true;

    Button searchBtn(font, "Search", 220, 330, 160, 50);

    const char* monthNames[] = {
        "January","February","March","April","May","June",
        "July","August","September","October","November","December"
    };

    // Main loop: selection UI
    while (selectWin.isOpen()) {
        while (auto ev = selectWin.pollEvent()) {
            if (ev->is<sf::Event::Closed>())
                selectWin.close();

            else if (ev->is<sf::Event::MouseButtonPressed>()) {
                auto mb = ev->getIf<sf::Event::MouseButtonPressed>();

                if (mb->button == sf::Mouse::Button::Left) {
                    float mx = float(mb->position.x);
                    float my = float(mb->position.y);

                    // priority
                    for (size_t i = 0; i < priority.size(); ++i)
                        if (priority[i].toggleIfClicked(mx, my))
                            for (size_t j = 0; j < priority.size(); ++j)
                                priority[j].checked = (j == i);

                    // algorithm
                    for (size_t i = 0; i < algo.size(); ++i)
                        if (algo[i].toggleIfClicked(mx, my))
                            for (size_t j = 0; j < algo.size(); ++j)
                                algo[j].checked = (j == i);

                    // click the search button
                    if (searchBtn.isClicked(mx, my)) {
                        DayList days;
                        DayList::SortMode mode = DayList::BY_CAPACITY;

                        if (priority[1].checked) mode = DayList::BY_WEATHER;

                        if (priority[2].checked) mode = DayList::BY_BOTH;
                        bool useQuick = algo[1].checked;
                        days.sort(mode, useQuick);

                        std::vector<Result> results;
                        for (auto* d = days.getHead(); d && results.size() < 10; d = d->next) {
                            if (!d->isBeforeNoon || !d->isOpen) continue;

                            results.push_back({
                                d->park, d->month, d->day, d->capacity, d->weather });
                        }

                        // Results window
                        sf::Vector2u resSize(600u, 400u);
                        sf::RenderWindow resWin(
                            sf::VideoMode(resSize),
                            sf::String("Top Days to Visit")
                        );

                        const char* weatherNames[] = {"Sunny","Cloudy","Rainy","Thunder","Snow"};

                        while (resWin.isOpen()) {
                            while (auto e2 = resWin.pollEvent())
                                if (e2->is<sf::Event::Closed>())
                                    resWin.close();

                            resWin.clear(sf::Color::White);

                            // Header
                            sf::Text hdr(font, sf::String("Top Days to Visit"), 24);
                            hdr.setFillColor(sf::Color::Black);
                            hdr.setPosition({20,20});
                            resWin.draw(hdr);

                            // Column labels
                            sf::Text
                            c1(font, sf::String("Park"), 18),
                            c2(font, sf::String("Date"), 18),
                            c3(font, sf::String("Capacity"), 18),
                            c4(font, sf::String("Weather"), 18);

                            c1.setFillColor(sf::Color::Black);
                            c2.setFillColor(sf::Color::Black);
                            c3.setFillColor(sf::Color::Black);
                            c4.setFillColor(sf::Color::Black);

                            c1.setPosition({20, 60});
                            c2.setPosition({260,60});
                            c3.setPosition({383,60});
                            c4.setPosition({500,60});

                            resWin.draw(c1);
                            resWin.draw(c2);
                            resWin.draw(c3);
                            resWin.draw(c4);

                            // Rows
                            float y = 100;

                            for (auto& r : results) {
                                sf::Text t1(font, sf::String(r.park), 16);
                                t1.setFillColor(sf::Color::Black);
                                t1.setPosition({20,y}); resWin.draw(t1);

                                std::string date = std::string(monthNames[r.month - 1]) + ", " + std::to_string(r.day);
                                sf::Text t2(font, sf::String(date), 16);

                                t2.setFillColor(sf::Color::Black);
                                t2.setPosition({260,y}); resWin.draw(t2);

                                std::string cap = std::to_string(r.capacity).substr(0,4);
                                sf::Text t3(font, sf::String(cap), 16);

                                t3.setFillColor(sf::Color::Black);
                                t3.setPosition({385,y}); resWin.draw(t3);

                                sf::Text t4(font, sf::String(weatherNames[r.weather]), 16);

                                t4.setFillColor(sf::Color::Black);
                                t4.setPosition({500,y}); resWin.draw(t4);

                                y += 30;
                            }

                            resWin.display();
                        }
                    }
                }
            }
        }

        // Draw selection UI
        selectWin.clear(sf::Color::White);

        // Main title
        sf::Text mainTitle(font, sf::String("Theme Park Crowd Calendar"), 24);
        mainTitle.setFillColor(sf::Color::Black);
        mainTitle.setPosition({20,20});
        selectWin.draw(mainTitle);

        // Subtitle
        sf::Text subtitle(font, sf::String("Select Priority"), 18);
        subtitle.setFillColor(sf::Color::Black);
        subtitle.setPosition({20,60});
        selectWin.draw(subtitle);

        // Priority
        for (auto& cb : priority) cb.draw(selectWin);

        // Algorithm title
        sf::Text algoTitle(font, sf::String("Select Algorithm"), 18);
        algoTitle.setFillColor(sf::Color::Black);
        algoTitle.setPosition({20,230});
        selectWin.draw(algoTitle);

        // Algorithm options
        for (auto& cb : algo)     cb.draw(selectWin);

        // Search button
        searchBtn.draw(selectWin);

        selectWin.display();
    }

    return 0;
}
