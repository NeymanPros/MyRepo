use iced::{Element, Application, Settings, Theme, executor, Command, font}; //my first iced app
use iced::widget::{text, text_editor, button, column, row};

fn main () -> iced::Result {
    Counter::run(Settings::default())
}

struct Counter{
    first: text_editor::Content,
    second: text_editor::Content,
    del: char,
    result: String
}

#[derive(Debug, Clone)]
enum Message{
    Edit1(text_editor::Action),
    Edit2(text_editor::Action),
    Delimetr,
    Count
}

impl Application for Counter{
    type Executor = executor::Default;
    type Message = Message;
    type Theme = Theme;
    type Flags = ();

    fn new(_flags: Self::Flags) -> (Self, Command<Message>) {
        (
            Self {
                first: text_editor::Content::new(),
                second: text_editor::Content::new(),
                del: "+".chars().nth(0).unwrap(),
                result: String::new()
            },
            Command::none()
        )
    }

    fn title(&self) -> String {
        String::from("Count to 4!")
    }

    fn update(&mut self, message: Message) -> Command<Message>{
        match message {
            Message::Edit1(action) => {
                self.first.perform(action);
            }
            Message::Edit2(action) => {
                self.second.perform(action);
            }
            Message::Delimetr => {
                match self.del {
                    '+' => {
                        self.del = '-';
                    }
                    '-' => {
                        self.del = '*'
                    }
                    '*' => {
                        self.del = '/'
                    }
                    _ => {
                        self.del = '+'
                    }
                }

            }
            Message::Count => {
                self.result = String::default();
                let mut a:i128 = 0;
                let mut b:i128 = 0;
                for _i in self.first.text().trim().chars() {
                    if _i >= '0' && _i <= '9' {
                        a = a * 10 + _i as i128 - '0' as i128;
                    }
                }
                self.result.push(self.del);
                for _i in self.second.text().trim().chars() {
                    if _i >= '0' && _i <= '9' {
                        b = b * 10 + _i as i128 - '0' as i128
                    }
                }
                self.first = text_editor::Content::with_text(a.to_string().as_str());
                self.second = text_editor::Content::with_text(b.to_string().as_str());
                if self.del == '+' {
                    self.result = (a + b).to_string()
                } else if self.del == '-'{
                    self.result = (a - b).to_string()
                } else if self.del == '*'{
                    self.result = (a * b).to_string()
                } else {
                    self.result = (a as f64 / b as f64).to_string()
                }
            }
        }

        Command::none()
    }

    fn view(&self) -> Element<'_, Self::Message> {
        column![text_editor(&self.first).on_action(Message::Edit1).font(font::Font::MONOSPACE),
            row![iced::widget::button("O").on_press(Message::Delimetr), "    ", iced::widget::text(self.del)],
            iced::widget::text_editor(&self.second).on_action(Message::Edit2).font(font::Font::MONOSPACE),
            iced::widget::button("=").on_press(Message::Count).width(150),
            iced::widget::text(self.result.clone())
        ].padding(10).into()
    }

    fn theme(&self) -> Theme {
        Theme::Nord
    }
}
