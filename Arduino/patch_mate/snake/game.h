
#pragma once

#include "../debug.h"

#include "pos.h"
#include "graphic.h"
#include "snake.h"
#include "direction.h"

namespace snake {

struct Game {
    void new_apple() {
        bool valid = true;
        do {
            apple_.set(rand() % Graphic::WIDTH, rand() % Graphic::HEIGHT);
            valid = true;

            for (uint8_t i = 0; i < snake_.length; ++i) {
                if (apple_ == snake_[i]) {
                    valid = false;
                    break;
                }
            }
        } while (!valid);
    }

    void start(unsigned long t) {
        debug(4, "snake start");

        srand(micros());
        step_interval_ = 1000;
        started_ = true;
        last_step_ = t;

        snake_.reset();
        direction_ = Direction::RIGHT;

        new_apple();

        step_move_made_ = false;

        graphic_.clear();
        for (uint8_t i = 0; i < snake_.length; ++i) {
            graphic_.set_item(snake_[i], Graphic::SNAKE);
        }
        graphic_.set_item(apple_, Graphic::APPLE);
        graphic_.flush();
    }

    void lose() {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Boo! You lose!");
        lcd.setCursor(0, 1);
        lcd.print("Length: ");
        lcd.setCursor(8, 1);
        lcd.print(snake_.length);

        started_ = false;
    }

    void win() {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("You won. Congratz!");
        lcd.setCursor(0, 1);
        lcd.print("Length: ");
        lcd.setCursor(8, 1);
        lcd.print(snake_.length);

        started_ = false;
    }

    void step() {
        if (!started_) return; // Game over. Don't bother calculating game logic.

        Pos head = snake_.head();

        // Calculate the head movement
        direction_.move(head);

        // Look for wall collision
        if (head.x >= Graphic::WIDTH || head.y >= Graphic::HEIGHT) {
            debug(4, "snake lose: wall collision ", head.x, head.y);

            lose();
            return;
        }

        // Calculate the movement of the tail
        //   We intentionally use i = snake_.length instead of
        //   i = snake_.length - 1 so that the snake will be lengthened right
        //   after it eats the apple_.
        for (uint8_t i = snake_.length; i >= 1; --i) {
            snake_[i] = snake_[i - 1];

            // Look for self collision
            if (head == snake_[i]) {
                debug(4, "snake lose: self collision [", i, "]: ", head.x, head.y);

                lose();
                return;
            }
        }

        // Update head position
        snake_.head() = head;

        graphic_.update_item(head, Graphic::SNAKE);

        if (apple_ == head) {
            ++snake_.length;
            step_interval_ = step_interval_ * 9 / 10;
            if (snake_.length >= Graphic::HEIGHT * Graphic::WIDTH) {
                win();
                return;
            }

            new_apple();
            graphic_.update_item(apple_, Graphic::APPLE);
        } else {
            graphic_.update_item(snake_[snake_.length], Graphic::NONE);
        }

    }

    void turn_left(unsigned long t) {
        if (step_move_made_) return;

        if (started_) {
            debug(5, "turn left");
            direction_.turn_left();
            step_move_made_ = true;
        } else {
            start(t);
        }
    }

    void turn_right(unsigned long t) {
        if (step_move_made_) return;

        if (started_) {
            debug(5, "turn right");
            direction_.turn_right();
            step_move_made_ = true;
        } else {
            start(t);
        }
    }

    void setup() {
        graphic_.setup();
        started_ = false;
    }

    void update(unsigned long t) {
        if (t - last_step_ > step_interval_) {
            step();
            last_step_ = t;
            step_move_made_ = false;
        }
    }

private:
    unsigned long last_step_;
    unsigned long step_interval_;
    bool step_move_made_;
    bool started_;

    Pos apple_;
    Snake snake_;
    Direction direction_;
    Graphic graphic_;
};

}
