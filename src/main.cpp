#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
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
            x + (w - b.size.x)/2 - b.position.x,
            y + (h - b.size.y)/2 - b.position.y
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

struct Result {
    std::string park;
    unsigned int month;
    unsigned int day;
    float capacity;
    unsigned int weather;
};

int main() {
    // Selection window
    sf::Vector2u selectSize(600u, 400u);
    sf::RenderWindow selectWin(sf::VideoMode(selectSize), sf::String("Select Priority"));
    selectWin.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("src/arial.ttf")) {
        return -1;
    }

    // Park dropdown setup (right side)
    std::vector<std::string> parkOptions = {
        "All Parks",
        "Magic Kingdom",
        "Disneyland",
        "Universal Islands of Adventure",
        "Disney's Hollywood Studios",
        "Universal Studios Florida",
        "Epcot",
        "Disney's Animal Kingdom",
        "Disney California Adventure",
        "Universal Studios Hollywood",
        "SeaWorld Orlando",
        "Busch Gardens Tampa Bay",
        "Knott's Berry Farm",
        "SeaWorld San Diego",
        "Cedar Point",
        "Kings Island"
    };
    std::string selectedPark = "All Parks";
    bool parkMenuOpen = false;

    sf::RectangleShape parkBox({200, 30});
    parkBox.setPosition({360, 90});
    parkBox.setFillColor(sf::Color::White);
    parkBox.setOutlineColor(sf::Color::Black);
    parkBox.setOutlineThickness(2);

    sf::Text parkText(font, sf::String(selectedPark), 16);
    parkText.setFillColor(sf::Color::Black);
    parkText.setPosition({365, 95});

    sf::Text parkLabel(font, sf::String("Select Park"), 16);
    parkLabel.setFillColor(sf::Color::Black);
    parkLabel.setPosition({360, 60});

    // Priority checkboxes
    std::vector<Checkbox> priority = {
        Checkbox(font, "By Capacity", 20,  90),
        Checkbox(font, "By Weather", 20, 130),
        Checkbox(font, "By Both", 20, 170)
    };

    priority[0].checked = true;

    // Algorithm checkboxes
    sf::Text algoTitle(font, sf::String("Select Algorithm"), 18);
    algoTitle.setFillColor(sf::Color::Black);
    algoTitle.setPosition({20.f, 230.f});
    std::vector<Checkbox> algo = {
        Checkbox(font, "Heap Sort" , 20, 260),
        Checkbox(font, "Quick Sort", 20, 300)
    };

    algo[0].checked = true;

    // Search button (bottom-center)
    Button searchBtn(font, "Search", 220, 330, 160, 50);

    // Main loop: selection UI
    while (selectWin.isOpen()) {
        while (auto ev = selectWin.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) {
                selectWin.close();
            }

            else if (ev->is<sf::Event::MouseButtonPressed>()) {
                auto mb = ev->getIf<sf::Event::MouseButtonPressed>();

                if (mb->button != sf::Mouse::Button::Left)
                    continue;

                float mx = float(mb->position.x);
                float my = float(mb->position.y);

                // Park dropdown toggle & selection
                if (parkBox.getGlobalBounds().contains({mx, my})) {
                    parkMenuOpen = !parkMenuOpen;
                }

                else if (parkMenuOpen) {
                    for (size_t i = 0; i < parkOptions.size(); ++i) {
                        sf::FloatRect r(
                            {360, 90 + (i+1)*30.f}, {200, 30});

                        if (r.contains({mx, my})) {
                            selectedPark = parkOptions[i];
                            parkText.setString(selectedPark);
                            parkMenuOpen = false;

                            break;
                        }
                    }
                }

                // Priority
                for (size_t i = 0; i < priority.size(); ++i) {
                    if (priority[i].toggleIfClicked(mx, my)) {
                        for (auto& cb : priority) cb.checked = false;
                        priority[i].checked = true;

                        break;
                    }
                }

                // Algorithm
                for (size_t i = 0; i < algo.size(); ++i) {
                    if (algo[i].toggleIfClicked(mx, my)) {
                        for (auto& cb : algo) cb.checked = false;
                        algo[i].checked = true;

                        break;
                    }
                }

                // click the search button
                if (searchBtn.isClicked(mx, my)) {
                    DayList days;
                    DayList::SortMode mode = DayList::BY_CAPACITY;

                    if (priority[1].checked)  mode = DayList::BY_WEATHER;

                    if (priority[2].checked)  mode = DayList::BY_BOTH;

                    bool useQuick = algo[1].checked;
                    days.sort(mode, useQuick);

                    // collect results
                    std::vector<Result> results;
                    for (auto* d = days.getHead(); d && results.size() < 10; d = d->next) {
                        if (!d->isBeforeNoon || !d->isOpen)
                            continue;

                        if (selectedPark != "All Parks" && d->park != selectedPark)
                            continue;

                        results.push_back({
                            d->park, d->month, d->day, d->capacity, d->weather});
                    }

                    // Results window
                    sf::Vector2u resSize(600u, 400u);
                    sf::RenderWindow resWin(sf::VideoMode(resSize), sf::String("Top Days to Visit"));
                    resWin.setFramerateLimit(30);

                    // Keep the results window open until the user closes it
                    while (resWin.isOpen()) {
                        while (auto e2 = resWin.pollEvent()) {
                            if (e2->is<sf::Event::Closed>())
                                resWin.close();
                        }

                        resWin.clear(sf::Color::White);

                        // header & columns
                        sf::Text hdr(font, sf::String("Top Days to Visit"), 24);
                        hdr.setFillColor(sf::Color::Black);
                        hdr.setPosition({20.f, 20.f});
                        resWin.draw(hdr);

                        sf::Text c1(font, sf::String("Park"),     18);
                        sf::Text c2(font, sf::String("Date"),     18);
                        sf::Text c3(font, sf::String("Capacity"), 18);
                        sf::Text c4(font, sf::String("Weather"),  18);

                        c1.setFillColor(sf::Color::Black);
                        c2.setFillColor(sf::Color::Black);
                        c3.setFillColor(sf::Color::Black);
                        c4.setFillColor(sf::Color::Black);

                        c1.setPosition({20.f, 60.f});
                        c2.setPosition({260.f, 60.f});
                        c3.setPosition({383.f, 60.f});
                        c4.setPosition({500.f, 60.f});

                        resWin.draw(c1);
                        resWin.draw(c2);
                        resWin.draw(c3);
                        resWin.draw(c4);

                        // draw rows...
                        float y = 100.f;
                        const char* monthNames[] = {
                            "January",
                            "February",
                            "March",
                            "April",
                            "May",
                            "June",
                            "July",
                            "August",
                            "September",
                            "October",
                            "November",
                            "December"
                        };

                        const char* weatherNames[] = {
                            "Sunny",
                            "Cloudy",
                            "Rainy",
                            "Thunder",
                            "Snow"
                        };

                        for (auto& r : results) {
                            sf::Text t1(font, sf::String(r.park), 16);
                            t1.setFillColor(sf::Color::Black);
                            t1.setPosition({20.f, y});
                            resWin.draw(t1);

                            std::string date = std::string(monthNames[r.month - 1]) + ", " + std::to_string(r.day);
                            sf::Text t2(font, sf::String(date), 16);
                            t2.setFillColor(sf::Color::Black);
                            t2.setPosition({260.f, y});
                            resWin.draw(t2);

                            int percentage = int(r.capacity * 100 + 0.5f);
                            std::string cap = std::to_string(percentage) + "%";

                            sf::Text t3(font, sf::String(cap), 16);
                            t3.setFillColor(sf::Color::Black);
                            t3.setPosition({385.f, y});
                            resWin.draw(t3);

                            sf::Text t4(font, sf::String(weatherNames[r.weather]), 16);
                            t4.setFillColor(sf::Color::Black);
                            t4.setPosition({500.f, y});
                            resWin.draw(t4);

                            y += 30.f;
                            if (y > 360.f) break;
                        }

                        resWin.display();
                    }
                }
            }
        }

        // Draw selection UI
        selectWin.clear(sf::Color::White);
        sf::Text mainTitle(font, sf::String("Theme Park Crowd Calendar"), 24);
        mainTitle.setFillColor(sf::Color::Black);
        mainTitle.setPosition({20,20});
        selectWin.draw(mainTitle);

        sf::Text subtitle(font, sf::String("Select Priority"), 18);
        subtitle.setFillColor(sf::Color::Black);
        subtitle.setPosition({20,60});
        selectWin.draw(subtitle);

        // park dropdown
        selectWin.draw(parkLabel);
        selectWin.draw(parkBox);
        selectWin.draw(parkText);

        if (parkMenuOpen) {
            for (size_t i = 0; i < parkOptions.size(); ++i) {
                sf::RectangleShape item({200,30});
                item.setPosition({360, 120.f + i*30.f});
                item.setFillColor(sf::Color::White);
                item.setOutlineColor(sf::Color::Black);
                item.setOutlineThickness(1);
                selectWin.draw(item);

                sf::Text it(font, sf::String(parkOptions[i]), 16);
                it.setFillColor(sf::Color::Black);
                it.setPosition({365, 125 + i*30.f});
                selectWin.draw(it);
            }
        }

        for (auto& cb : priority) cb.draw(selectWin);
        selectWin.draw(algoTitle);

        for (auto& cb : algo) {
            cb.draw(selectWin);
        }

        searchBtn.draw(selectWin);
        selectWin.display();
    }

    return 0;
}
