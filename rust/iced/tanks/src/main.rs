use iced::{application, Center, Color, Element, Fill, FillPortion, Left, Point, Rectangle, Renderer, Size, Subscription, Theme, time, Vector};
use iced::keyboard::{Key, on_key_press, on_key_release};
use iced::mouse::Cursor;
use iced::widget::{text, pick_list, Column, row, button, canvas, horizontal_space, ProgressBar, vertical_space};
use iced::widget::canvas::Geometry;
use rand::random;

trait Creature {
    fn get_location(&self) -> Point {
        Point{x: 0f32, y: 0f32}
    }
    fn set_location(&mut self, _value: Point) {}
    fn get_radius(&self) -> f32{
        0f32
    }
    fn walk(&mut self, direction: Vector){
        self.set_location(self.get_location() + direction);
    }
}

trait Shut: Creature {
    fn shut(&self, bullets: &mut Vec<Bullet>, direction: Vector) {
        bullets.push(Bullet::create(self.get_location() + direction * self.get_radius(), direction));
    }
}

struct Character {
    hp: f32,
    speed: f32,
    location: Point,
    direction: Vector,
    radius: f32
}
impl Creature for Character {
    fn get_location(&self) -> Point {
        self.location
    }
    fn set_location(&mut self, value: Point) {
        self.location = value;
    }
    fn get_radius(&self) -> f32 {
        self.radius
    }
}
impl Shut for Character {}

struct Enemy {
    hp: f32,
    speed: f32,
    location: Point,
    radius: f32
}
impl Creature for Enemy {
    fn get_location(&self) -> Point {
        self.location
    }
    fn set_location(&mut self, value: Point) {
        self.location = value;
    }
    fn get_radius(&self) -> f32 {
        self.radius
    }
}
impl Shut for Enemy {}
impl Enemy {
    fn trace_player(&self, player_location: Point) -> Vector {
        if (player_location.x - self.location.x).abs() >= (player_location.y - self.location.y).abs() {
            Vector{x: (player_location.x - self.location.x).signum(), y: 0f32}
        } else {
            Vector{x: 0f32, y: (player_location.y - self.location.y).signum()}
        }
    }
}
impl Clone for Enemy{
    fn clone(&self) -> Self {
        Enemy{hp: self.hp, speed: self.speed, location: self.location, radius: self.radius}
    }
}

struct Bullet {
    speed: f32,
    location: Point,
    direction: Vector,
    radius: f32
}
impl Creature for Bullet {
    fn get_location(&self) -> Point {
        self.location
    }
    fn set_location(&mut self, value: Point) {
        self.location = value;
    }
}
impl Bullet {
    fn create(from: Point, to: Vector) -> Self {
        Self {
            speed: 200f32,
            location: from,
            direction: to,
            radius: 2f32
        }
    }
}
impl Clone for Bullet {
    fn clone(&self) -> Self {
        Bullet{speed: self.speed, location: self.location, direction: self.direction, radius: self.radius}
    }
}

struct Field {
    size: Size,
    player_location: Point,
    player_radius: f32,
    enemies: Vec<Enemy>,
    bullets: Vec<Bullet>
}

impl<Message> canvas::Program<Message> for Field {
    type State = ();

    fn draw(&self, _state: &Self::State, renderer: &Renderer, _theme: &Theme, bounds: Rectangle, _cursor: Cursor) -> Vec<Geometry<Renderer>> {
        let mut frame = canvas::Frame::new(renderer, bounds.size());
        let mut add = canvas::Path::rectangle(Point{x: 0f32, y: 0f32}, self.size);

        frame.fill(&add, Color::WHITE);

        add = canvas::Path::circle(self.player_location, self.player_radius);
        frame.fill(&add, Color::from_rgb(0f32, 128f32, 0f32));

        for i in self.enemies.clone() {
            add = canvas::Path::circle(i.location, i.radius);
            frame.fill(&add, Color::BLACK);
        }

        for i in self.bullets.clone() {
            add = canvas::Path::circle(i.location, i.radius);
            frame.fill(&add, Color::from_rgb(255f32, 0f32, 0f32));
        }

        vec![frame.into_geometry()]
    }
}


struct Tanks {
    status: &'static str,
    size: Size,
    size_to_pick: [f32; 3],
    player: Character,
    player_move: bool,
    player_shut: bool,
    enemies: Vec<Enemy>,
    bullets: Vec<Bullet>,
    obstacles: Vec<Point>,
    common_radius: f32
}

#[derive(Debug, Clone)]
enum Message{
    ChangeField(char, f32),
    GenerateMap,
    Tick,
    PlayerMove(bool, Vector),
    PlayerShut
}

impl Tanks {
    fn update(&mut self, message: Message) {
        match message {
            Message::ChangeField(x, value) => {
                if x == 'x' {
                    self.size.width = value
                }
                else {
                    self.size.height = value
                }
            }
            Message::GenerateMap => {
                self.enemies.push(Enemy{hp: 1000f32, speed: 50f32, location: Point{x: self.size.width - self.common_radius, y: self.common_radius}, radius: self.common_radius});
                self.enemies.push(Enemy{hp: 1000f32, speed: 50f32, location: Point{x: self.common_radius, y: self.size.height - self.common_radius}, radius: self.common_radius});
                self.status = "in game"
            }
            Message::PlayerMove(acting, new_direction) => {
                if !self.player_move && acting {
                    self.player_move = acting;
                    self.player.direction = new_direction;
                }
                else if self.player_move && !acting {
                    if self.player.direction == new_direction {
                        self.player_move = false;
                    }
                }
                else if self.player_move && acting {
                    self.player.direction = new_direction
                }
            }
            Message::PlayerShut => {
                self.player_shut = true;
            }
            Message::Tick => {
                for mut i in &mut self.enemies {
                    i.walk(i.trace_player(self.player.get_location()) * (i.speed / 64f32));
                    if random::<i16>() % 64i16 == 0 {
                        i.shut(&mut self.bullets, i.trace_player(self.player.get_location()))
                    }
                }
                if self.player_move {
                    self.player.walk(self.player.direction * (self.player.speed / 64f32));
                    {
                        if self.player.location.x < self.player.radius {
                            self.player.location.x = self.player.radius
                        }
                        if self.player.location.y < self.player.radius {
                            self.player.location.y = self.player.radius
                        }
                        if self.player.location.y > self.size.height - self.player.radius {
                            self.player.location.y = self.size.height - self.player.radius
                        }
                        if self.player.location.x > self.size.width - self.player.radius {
                            self.player.location.x = self.size.width - self.player.radius
                        }
                    }
                }
                if self.player_shut {
                    self.player_shut = false;
                    self.player.shut(&mut self.bullets, self.player.direction);
                }
                let mut rm_ed:usize = 0;
                for i in 0..self.bullets.len() {

                    if self.bullets[i - rm_ed].location.x < self.bullets[i - rm_ed].radius || self.bullets[i - rm_ed].location.y < self.bullets[i - rm_ed].radius ||
                        self.bullets[i - rm_ed].location.x > self.size.width - self.bullets[i - rm_ed].radius || self.bullets[i - rm_ed].location.y > self.size.height - self.bullets[i - rm_ed].radius
                    {
                        self.bullets.remove(i - rm_ed);
                        rm_ed += 1;
                    }
                    else {
                        let dir = self.bullets[i - rm_ed].direction * (self.bullets[i - rm_ed].speed / 64f32);
                        self.bullets[i - rm_ed].walk(dir);
                    }
                }

                rm_ed = 0;
                for i in 0..self.bullets.len() {
                    if self.player.location.distance(self.bullets[i - rm_ed].location) < self.player.radius + self.bullets[i - rm_ed].radius {
                        self.player.hp -= 10f32;
                        self.bullets.remove(i - rm_ed);
                        rm_ed += 1;
                    }
                    else {
                        for j in &mut self.enemies {
                            if j.location.distance(self.bullets[i - rm_ed].location) < j.radius + self.bullets[i - rm_ed].radius {
                                j.hp -= 10f32;
                                self.bullets.remove(i - rm_ed);
                                rm_ed += 1;
                                break;
                            }
                        }
                    }
                }

                if self.player.hp == 0f32 || self.enemies.len() == 0 {
                    self.status = "menu";
                }
                else {
                    rm_ed = 0;
                    for i in 0..self.enemies.len() {
                        if self.enemies[i - rm_ed].hp < 1f32 {
                            self.enemies.remove(i - rm_ed);
                            rm_ed += 1;
                        }
                    }
                }
                 
                if self.status == "menu" {
                    self.enemies.clear();
                    self.bullets.clear();
                    self.player.hp = 100f32;
                }
            }
        }
    }

    fn view(&self) -> Element<Message> {
        if self.status == "menu" {
            let mut settings = Column::new();
            settings = settings.push(text("Tanks Offline").width(Fill).align_x(Center));
            settings = settings.push(row!(horizontal_space().width(FillPortion(1)), pick_list(self.size_to_pick, Some(self.size.width), |value| {Message::ChangeField('x', value)}), text("  x  "),
                                    pick_list(self.size_to_pick, Some(self.size.height), |value| {Message::ChangeField('y', value)}), horizontal_space().width(FillPortion(1))).width(Fill));
            settings = settings.push(row![horizontal_space().width(FillPortion(1)), button("Start").on_press(Message::GenerateMap), horizontal_space().width(FillPortion(1))]);
            settings.into()
        }
        else if self.status == "in game" {
            let mut field = Column::new();
            field = field.push(row![horizontal_space().width(FillPortion(1)), text("HP: "), ProgressBar::new(0f32..=100f32, self.player.hp).height(10).width(FillPortion(1))].align_y(Center)).align_x(Left);
            field = field.push(row![horizontal_space(), canvas(Field{size: self.size, player_location: self.player.get_location(), player_radius: self.player.get_radius(),
                enemies: self.enemies.to_vec(), bullets: self.bullets.to_vec()}).width(self.size.width).height(self.size.height), horizontal_space()]).width(Fill);
            field.width(Fill).height(Fill).padding(50).into()
        }

        else {
            text("Hello!").into()
        }
    }

    fn theme (&self) -> Theme {
        Theme::SolarizedDark
    }

    fn subscription(&self) -> Subscription<Message>{
        if self.status == "in game" {
            fn key_move (key: Key, acting: bool) -> Option<Message> {
                    match key.as_ref() {
                        Key::Character("w") => Some(Message::PlayerMove(acting, Vector{x: 0f32, y: -1f32})),
                        Key::Character("s") => Some(Message::PlayerMove(acting, Vector{x: 0f32, y: 1f32})),
                        Key::Character("a") => Some(Message::PlayerMove(acting, Vector{x: -1f32, y: 0f32})),
                        Key::Character("d") => Some(Message::PlayerMove(acting, Vector{x: 1f32, y: 0f32})),
                        Key::Named(iced::keyboard::key::Named::Space) => { if acting { Some(Message::PlayerShut) } else { None } },
                        _ => None
                    }

            }
            Subscription::batch(vec![on_key_press(|key, _modifiers| key_move(key, true)), on_key_release(|key, _modifiers| key_move(key, false)),
                                     time::every(time::Duration::from_millis(1000/64)).map(|_| Message::Tick)])
        }
        else {
           Subscription::none()
        }
    }
}

impl Default for Tanks {
    fn default() -> Self {
        Self{
            status: "menu",
            size: Size{width: 500f32, height: 500f32},
            size_to_pick: [500f32, 750f32, 1000f32],
            player: Character{hp: 100f32, speed: 100f32, location: Point{x: 0f32, y: 0f32}, direction: Vector{x: 0f32, y:1f32}, radius: 10f32},
            player_move: false,
            player_shut: false,
            enemies: Vec::new(),
            bullets: Vec::new(),
            obstacles: Vec::new(),
            common_radius: 10f32
        }
    }
}

fn main() -> iced::Result {
    application("tanks", Tanks::update, Tanks::view).theme(Tanks::theme).subscription(Tanks::subscription).run()
}
