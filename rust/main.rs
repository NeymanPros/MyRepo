use iced::{Application, Command, Element, executor, Renderer, Settings, Subscription, Theme, time};
use iced::widget::{text, ProgressBar, Column, Button, row};
use std::time::{SystemTime};

fn place () -> [f32; 100] {
    let mut a: [f32; 100] = [0.0; 100];
    let seed = SystemTime::now().duration_since(SystemTime::UNIX_EPOCH).unwrap().as_millis();
    for i in 0..100 {
        a[i] = ((seed - i as u128 % 5 * 20 + i as u128 / 5) % 100) as f32;
    }
    a
}

fn spawn_bar (mut a:f32) -> ProgressBar {
    if a < 0f32 || a > 100f32 {
        a = 0f32
    }
    ProgressBar::new(0f32..=100f32, a).height(5)
}

fn bubble (a: &mut [f32; 100], in_process: &mut bool) {
    *in_process = false;
    for i in 1..a.len() {
        if a[i] < a[i - 1] {
            a.swap(i, i - 1);
            if !*in_process{
                *in_process = true;
            }
        }
    }
}

fn min_max (a: &mut [f32; 100], in_process: &mut bool, s: &mut usize) {
    let mut min: (f32, usize) = (100f32, 0usize);
    let mut max: (f32, usize) = (0f32, 0usize);

    for i in *s..a.len() - *s {
        if a[i] <= min.0 {
            min.0 = a[i];
            min.1 = i;
        }
        if a[i] >= max.0 {
            max.0 = a[i];
            max.1 = i;
        }
    }

    let size = a.len();
    if min.1 == size - 1 - *s && max.1 == *s {
        a.swap(*s, min.1)
    } else if min.1 == size - 1 - *s {
        a.swap(*s, min.1);
        a.swap(size - 1 - *s, max.1);
    } else {
        a.swap(size - 1 - *s, max.1);
        a.swap(*s, min.1);
    }

    *s += 1;
    if *s >= size / 2 {
        *in_process = false;
        *s = 0;
    }
}

fn order (a: &mut [f32; 100], in_process: &mut bool, s: &mut usize){
    for i in (0..*s).rev() {
        if a[i] > a[i + 1] {
            a.swap(i, i + 1)
        }
        else {
            break
        }
    }

    *s += 1;
    if *s > a.len() - 1 {
        *s = 0;
        *in_process = false
    }
}

struct Sorting {
    ob: [f32; 100],
    in_process: bool,
    active: String,
    s: usize //iter for algorithms
}

#[derive(Clone, Debug)]
enum Message {
    New,
    Begin(String),
    Sort
}

impl Application for Sorting {
    type Executor = executor::Default;
    type Message = Message;
    type Theme = Theme;
    type Flags = ();

    fn new(_flags: Self::Flags) -> (Self, Command<Self::Message>) {
        (
            Self {
                ob: place(),
                s: 0,
                in_process: false,
                active: String::from("Bubble")
            },
            Command::none()
        )
    }

    fn title(&self) -> String {
        String::from("Sort")
    }

    fn update(&mut self, message: Self::Message) -> Command<Self::Message> {
        match message {
            Message::New => {
                self.ob = place();
                self.in_process = false;
                self.s = 0
            }
            Message::Begin(call_for) => {
                self.in_process = true;
                self.s = 0;
                self.active = call_for
            }
            Message::Sort => {
                if self.in_process {
                    if self.active == "Bubble" {
                        bubble(&mut self.ob, &mut self.in_process)
                    } else if self.active == "MinMax" {
                        min_max(&mut self.ob, &mut self.in_process, &mut self.s)
                    } else if self.active == "Order" {
                        order(&mut self.ob, &mut self.in_process, &mut self.s)
                    }
                }
            }
        }
        Command::none()
    }

    fn view(&self) -> Element<'_, Self::Message, Self::Theme, Renderer> {
        let mut a = Column::new();
        for i in 0..100 {
            a = a.push(spawn_bar(self.ob[i]));
            a = a.push(text("").height(2));
        };

        let bubble_sort:Button<'_, Self::Message> = Button::new("Bubble").on_press(Message::Begin(String::from("Bubble"))).width(200);
        let min_max_sort:Button<'_, Self::Message> = Button::new("Min - Max").on_press(Message::Begin(String::from("MinMax"))).width(200);
        let order_sort:Button<'_, Self::Message> = Button::new("Order").on_press(Message::Begin(String::from("Order"))).width(200);
        let new_bars:Button<'_, Self::Message> = Button::new("New bars").on_press(Message::New).width(200);
        a = a.push(row![bubble_sort, min_max_sort, order_sort, new_bars].spacing(100));
        a.padding(10).into()
    }

    fn theme(&self) -> Self::Theme {
        Theme::KanagawaWave
    }

    fn subscription(&self) -> Subscription<Self::Message> {
        time::every(time::Duration::from_millis(35)).map(|_| Message::Sort)
    }
}

fn main() -> iced::Result {
    Sorting::run(Settings::default())
}
