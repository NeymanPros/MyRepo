mod excel_parse;
use excel_parse::{excel_dots, excel_lines};

use iced::{Rectangle, Renderer, Theme, Point, Size, Color, Fill, Center, mouse, Subscription};
use iced::event::Status;
use iced::mouse::{Cursor, Interaction};
use iced::widget::{canvas, Column, column, row, container, horizontal_space, text, text_editor, button};
use iced::widget::canvas::{Geometry, Stroke, Path, Event};

/// Tools to draw Framework
#[derive(Clone, Debug, Default)]
struct Model {
    dots: Vec<Point>,
    lines: Vec<(Point, Point)>
}

impl Model {
    fn draw_model(model: &Model, frame: &mut canvas::Frame, scale: f32) {
        let mut point: Path;
        for i in 0..model.dots.len() {
            point = Path::circle(model.dots[i], scale * 2.0);
            frame.fill(&point, Color::BLACK);
        }

        let lines = Path::new(|p| {
            for i in &model.lines {
                p.move_to(i.0);
                p.line_to(i.1);
            }
        });

        frame.stroke(&lines, Stroke::default().with_color(Color::BLACK).with_width(scale));
    }

    fn find_point(&self, dot: Point, scale: f32) -> usize {
        let mut i: usize = 0;
        while i < self.dots.len() {
            if self.dots[i].distance(dot) < scale * 2.0 {
                return i;
            }
            i += 1;
        }
        i
    }
}

/// Structure that works with Model elements
#[derive(Debug, Clone, Copy)]
enum Drawing {
    Line {},
    LineDot { dot: Point },
    Dot {},
    SelectDot { dot: Point, num: usize },
    None {}
}

impl Default for Drawing {
    fn default() -> Self {
        Drawing::None {}
    }
}

impl Drawing {
    fn edit(&self, renderer: &Renderer, bounds: Rectangle, cursor: Cursor, scale: f32) -> Geometry<Renderer> {
        let mut frame = canvas::Frame::new(renderer, bounds.size());

        match *self {
            Drawing::LineDot { dot } => {
                let cursor_pos = cursor.position_in(bounds).unwrap_or(dot);
                frame.fill(&Path::circle(dot, scale * 4.0), Color::from_rgb8(255, 0, 0));
                frame.stroke(&Path::line(dot, cursor_pos), Stroke::default().with_width(2.0 * scale).with_color(Color::from_rgb8(255, 0, 0)));
            }
            Drawing::SelectDot { dot, .. } => {
                frame.fill(&Path::circle(dot, scale * 4.0), Color::from_rgb8(255, 0, 0));
            }
            _ => {}
        };

        frame.into_geometry()
    }
}

impl Drawing {
    fn as_str (&self) -> &'static str {
        match *self {
            Drawing::Dot {} => { "Dot" }
            Drawing::Line {} => { "Line" }
            Drawing::LineDot { .. } => { "Line" }
            _ => "Move"
        }
    }
}

/// Canvas, that draws a model
struct Framework<'a> {
    state: &'a State,
    model: &'a Model,
    scale: f32,
    mode: &'static str
}

impl canvas::Program<Model> for Framework<'_> {
    type State = Drawing;

    fn update(&self, state: &mut Self::State, event: Event, bounds: Rectangle, cursor: Cursor) -> (Status, Option<Model>) {
        let Some(cursor_pos) = cursor.position_in(bounds) else {
            return (Status::Ignored, None);
        };


        match event {
            Event::Mouse(mouse_event) => {
                let message = match mouse_event {
                    mouse::Event::ButtonPressed(mouse::Button::Left) => {
                        if state.as_str() != self.mode {
                            *state = match self.mode {
                                "Dot" => { Drawing::Dot {} }
                                "Line" => {
                                    match *state {
                                        Drawing::SelectDot { dot, .. } => {
                                            Drawing::LineDot { dot }
                                        }
                                        _ => { Drawing::Line {} }
                                    }
                                }
                                _ => Drawing::None {}
                            };
                        }
                        match *state {
                            Drawing::Dot {} => {
                                let a = self.model.find_point(cursor_pos, self.scale);
                                *state = Drawing::SelectDot { dot:cursor_pos, num: a };
                                Some(Model{dots: vec![cursor_pos], lines: vec![]})
                            }
                            Drawing::Line {} => {
                                *state = Drawing::LineDot{ dot: cursor_pos };

                                None
                            }
                            Drawing::LineDot { dot } => {
                                *state = Drawing::Line {};

                                Some(Model{dots: vec![dot, cursor_pos], lines: vec![(dot, cursor_pos)]})
                            }
                            _ => None
                        }
                    }
                    mouse::Event::ButtonPressed(mouse::Button::Right) => {
                        match *state {
                            Drawing::LineDot { .. } => {
                                None
                            }

                            _ => {
                                let a = self.model.find_point(cursor_pos, self.scale);
                                if a >= self.model.dots.len() {
                                    None
                                }
                                else {
                                    *state = Drawing::SelectDot { dot: cursor_pos, num: a };
                                    Some(Model{dots: vec![cursor_pos], lines: Vec::default()})
                                }
                            }
                        }

                    }
                    _ => None,
                };

                (Status::Captured, message)
            }
            _ => (Status::Ignored, None),
        }
    }

    fn draw(&self, state: &Self::State, renderer: &Renderer, _theme: &Theme, bounds: Rectangle, cursor: Cursor) -> Vec<Geometry> {
        let content = self.state.cache.draw(renderer, bounds.size(), |frame| {
            Model::draw_model(self.model, frame, self.scale);
        });

        vec![content, state.edit(renderer, bounds, cursor, self.scale)]
    }

    fn mouse_interaction(&self, _state: &Self::State, bounds: Rectangle, cursor: Cursor) -> Interaction {
        if cursor.is_over(bounds) {
            Interaction::Crosshair
        } else {
            Interaction::default()
        }
    }
}


/// canvas::Cache, contains already drawn Framework
#[derive(Default)]
pub struct State {
    cache: canvas::Cache,
}

impl State {
    fn view<'a>(&'a self, model: &'a Model, scale: f32, mode: &'static str) -> iced::Element<'a, Model> {
        canvas(Framework {
            state: self,
            model,
            scale,
            mode
        })
            .width(Fill)
            .height(Fill)
            .into()
    }

    pub fn request_redraw(&mut self) {
        self.cache.clear();
    }
}


struct VecRed {
    path_to_excel: text_editor::Content,
    modes: [&'static str; 3],
    mode: Drawing,
    chosen_dot: Option<(Point, usize)>,

    max_cords: Size,
    current_size: Size,
    state: State,
    model: Model,

    scale: f32
}


#[derive(Debug, Clone)]
enum Message {
    ChangeMode(&'static str),
    ClearAll,
    EditPath(text_editor::Action),

    Def(Model),
    GetData
}

impl VecRed {
    fn update(&mut self, message: Message) {
        match message {
            Message::ChangeMode(new_mode) => {
                match new_mode {
                    "Dot" => { self.mode = Drawing::Dot {} }
                    "Line" => { self.mode = Drawing::Line {} }
                    _ => { self.mode = Drawing::None {} }
                }

            }

            Message::EditPath(edited) => {
                self.path_to_excel.perform(edited)
            }

            Message::GetData => {
                //self.path_to_excel;
                self.model.dots = excel_dots();
                self.model.lines = excel_lines(&self.model.dots);
                self.state.request_redraw()
            }

            Message::Def(add_model) => {
                if add_model.dots.len() == 1 && add_model.lines.len() == 0 {
                    let a = self.model.find_point(add_model.dots[0], self.scale);
                    if a >= self.model.dots.len() {
                        self.model.dots.push(add_model.dots[0]);
                        self.state.request_redraw()
                    }
                    self.chosen_dot = Some((add_model.dots[0], a))
                } else {
                    for i in add_model.dots {
                        self.model.dots.push(i)
                    }
                    for i in add_model.lines {
                        self.model.lines.push(i)
                    }
                    self.state.request_redraw()
                }
            }

            Message::ClearAll => {
                self.model.dots.clear();
                self.model.lines.clear();
                self.chosen_dot = None;
                self.state.request_redraw()
            }

            /*Message::Resize(new_size) => {
                self.current_size = new_size * self.shrink_value;
                self.shrink_value = self.get_shrink() as f32;
                println!("{}, {} => {}", new_size.width, new_size.height, self.shrink_value);
            }*/
        }
    }

    fn view(&self) -> iced::Element<Message> {
        let map = container(self.state.view(&self.model, self.scale, self.mode.as_str()).map(Message::Def))
            .width(self.max_cords.width).height(self.max_cords.height);

        row![map, horizontal_space().height(Fill).width(10.0), self.side_panel().width(Fill).height(Fill)].into()
    }

    fn subscription(&self) -> Subscription<Message> {
        /*let resize_window = iced::window::resize_events();
        return resize_window.map(|param| -> Message {
            Message::Resize(param.1)
        });*/
        Subscription::none()
    }

    /*fn get_shrink (&self) -> f64 {
        if self.shrink_value * self.current_size.width / self.max_cords.width < self.shrink_value * self.current_size.height / self.max_cords.height {
            1.0/(self.current_size.width / self.max_cords.width) as f64
        }
        else {
            1.0/(self.current_size.height / self.max_cords.height) as f64
        }
    }*/
}

impl VecRed {
    fn side_panel (&self) -> Column<Message> {
        let mode = iced::widget::pick_list(self.modes, Some(self.mode.as_str()), Message::ChangeMode);
        let for_path = text_editor(&self.path_to_excel).on_action(Message::EditPath);
        let get_data = button("hello").on_press(Message::GetData);
        let clear_frame = button("Clear all").on_press(Message::ClearAll);
        let num= match self.chosen_dot {
            None => { String::from("" )}
            _ => { self.chosen_dot.unwrap().1.to_string() }
        };
        let dot_num = text("Number of dot: ".to_owned() + num.as_str());
        let dot_cords = row![column![text("X: "), text("Y: ")]/*, column![text_editor("")]*/];

        column![mode, for_path, get_data, clear_frame, dot_num, dot_cords].spacing(5).align_x(Center)
    }
}

impl Default for VecRed {
    fn default() -> Self {
        Self {
            path_to_excel: text_editor::Content::default(),
            modes: ["Move", "Dot", "Line"],
            mode: Drawing::None {},
            chosen_dot: None,

            max_cords: Size::new(950.0, 950.0),
            current_size: Size::new(1200.0, 950.0),
            state: State::default(),
            model: Model::default(),

            scale: 1.0
        }
    }
}

fn main() -> iced::Result {
    iced::application("VecRed", VecRed::update, VecRed::view).antialiasing(true).resizable(false)
        .window_size(Size::new(1200.0, 1000.0)).subscription(VecRed::subscription).run()
}
