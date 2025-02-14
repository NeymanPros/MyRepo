use iced::{Center, Color, Element, Fill, FillPortion, mouse, Subscription, Theme};
use iced::widget::{Text, Button, Column, Row, button, pick_list, Container};
use iced::event::Event::Mouse;
use iced::keyboard::on_key_press;

fn count_neigh(i: usize, j: usize, answer: &Vec<Vec<bool>>) -> i8 {
    let mut a: i8 = 0;
    for _i in i - 1..=i + 1 {
        for _j in j - 1..=j + 1 {
            if answer[_i][_j] {
                a += 1;
            }
        }
    }

    a
}

fn bomb_generator(size: usize, mut bombs: i16, clear_i: usize, clear_j: usize) -> Vec<Vec<bool>> {
    let mut answer: Vec<Vec<bool>> = vec!(vec!(false; size); size);

    while bombs > 0 {
        let i = rand::random::<usize>() % (size - 2) + 1;
        let j = rand::random::<usize>() % (size - 2) + 1;
        if !answer[i][j] && (i < clear_i - 1 || i > clear_i + 1 || j < clear_j - 1 || j > clear_j + 1) {
            answer[i][j] = true;
            bombs -= 1;
        }
    }

    answer
}

struct Sapper {
    status: String,
    pick_size: [usize; 3],
    size: usize,
    gaming_field: Vec<Vec<String>>,
    answer_field: Vec<Vec<bool>>,
    bombs: i16,
    opened: i16
}

#[derive(Debug, Clone)]
enum Message {
    ChangeSettings(usize),
    Start,
    Open(usize, usize)
}

impl Sapper {
    fn title (&self) -> String {
        String::from("Sapper")
    }

    fn update (&mut self, message: Message) {
        match message {
            Message::ChangeSettings(a) => {
                self.size = a;
                if self.status != "Pre-game" {
                    self.status = String::from("Pre-game");
                    self.opened = 0
                }
            }
            Message::Start => {
                self.gaming_field = vec!(vec!(String::default(); self.size + 2); self.size + 2);
                self.status = String::from("In game")

            }
            Message::Open(i, j) => {
                if self.opened == 0 {
                    self.bombs = (self.size * self.size) as i16 * 2 / 9;
                    self.answer_field = bomb_generator(self.size + 2, self.bombs.clone(), i, j);
                }
                if self.answer_field[i][j] {
                    self.status = String::from("Defeat");
                    self.answer_field = Vec::new();
                    self.gaming_field = Vec::new();
                }
                else if self.gaming_field[i][j] == String::default() {
                    self.gaming_field[i][j] = count_neigh(i, j, &self.answer_field.clone()).to_string();
                    self.opened += 1;
                    if self.bombs >= (self.size * self.size) as i16 - self.opened {
                        self.status = String::from("Victory");
                    }

                    if self.gaming_field[i][j] == "0" {
                        if i > 1 {
                            if j > 1 {
                                Self::update(self, Message::Open(i - 1, j - 1));
                                Self::update(self, Message::Open(i, j - 1));
                            }
                            Self::update(self, Message::Open(i - 1, j));
                            if j < self.size {
                                Self::update(self, Message::Open(i - 1, j + 1));
                                Self::update(self, Message::Open(i, j + 1));
                            }
                        }

                        if i < self.size {
                            if j > 1 {
                                Self::update(self, Message::Open(i + 1, j - 1));
                                Self::update(self, Message::Open(i, j - 1));
                            }
                            Self::update(self, Message::Open(i + 1, j));
                            if j < self.size {
                                Self::update(self, Message::Open(i + 1, j + 1));
                                Self::update(self, Message::Open(i, j + 1));
                            }
                        }
                    }
                }
            }
        }
    }

    fn view (&self) -> Element<Message> {
        let mut full: Column<Message> = Column::new();
        if self.status == "In game" {
            full = full.push(Text::new(self.status.as_str()).width(Fill).align_x(Center));

            for i in 1..self.gaming_field.len() - 1 {
                let mut row: Row<Message> = Row::new();
                for j in 1..self.gaming_field[i].len() - 1 {
                    if self.gaming_field[i][j] != String::default() {
                        row = row.push(button(self.gaming_field[i][j].as_str()).width(FillPortion(1)).height(FillPortion(1))).spacing(5);
                    } else {
                        row = row.push(Button::new("").on_press(Message::Open(i, j)).width(FillPortion(1)).height(FillPortion(1))).spacing(5)
                    }
                }

                full = full.push(row).spacing(5);
            }

            full = full.height(Fill).width(Fill)
        }
        else if self.status == "Pre-game" {
            full = full.push(pick_list(self.pick_size.clone(), Some(self.size), |a| { Message::ChangeSettings(a) }).text_size(20));
            full = full.push(Button::new("Start").height(40).on_press(Message::Start)).spacing(50);
        }
        else if self.status == "Victory" {
            full = full.push(Text::new("YOU WON AND SURVIVED").size(164).color(Color::from_rgb(0f32,255f32,0f32)))
        }
        else {
            full = full .push(Text::new("YOU LOST AND DIED").size(164).color(Color::from_rgb(255f32,0f32,0f32)))
        }

        //let out = Container::new(Text::new("")).height(Fill);
        Container::new(full.padding(50)).width(Fill).height(Fill).align_x(Center).align_y(Center).into()
    }

    fn theme(&self) -> Theme {
        Theme::Oxocarbon
    }

    fn subscription(&self) -> Subscription<Message> {
        if self.status == "Victory" || self.status == "Defeat" {
            on_key_press(|a, b| {Some(Message::ChangeSettings(10))})
        }
        else {
            Subscription::none()
        }
    }

}

impl Default for Sapper {
    fn default() -> Self {
        Self {
            status: String::from("Pre-game"),
            pick_size: [10, 15, 18],
            size: 10,
            answer_field: Vec::new(),
            gaming_field: Vec::new(),
            bombs: 20,
            opened: 0
        }
    }
}

fn main() -> iced::Result {
    iced::application(Sapper::title, Sapper::update, Sapper::view).subscription(Sapper::subscription).theme(Sapper::theme).run()
}
