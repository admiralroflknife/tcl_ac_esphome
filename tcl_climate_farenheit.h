#include "esphome.h"
#include <cmath>

class MyCustomClimate : public PollingComponent, public Climate, public UARTDevice {
public:

  MyCustomClimate(UARTComponent *parent) : UARTDevice(parent) {}
  MyCustomClimate() : PollingComponent() {}
  bool is_changed : 1;

  union get_cmd_resp_t{
    struct {
      uint8_t header;
      uint8_t byte_1;
      uint8_t byte_2;
      uint8_t type;
      uint8_t len;
      uint8_t byte_5;
      uint8_t byte_6;
      
      uint8_t mode : 4;
      uint8_t power : 1;
      uint8_t disp : 1;
      uint8_t eco : 1;
      uint8_t turbo : 1;

      uint8_t temp : 4;
      uint8_t fan : 3;
      uint8_t byte_8_bit_7 : 1;

      uint8_t byte_9;

      uint8_t byte_10_bit_0_4 : 5;
      uint8_t hswing : 1;
      uint8_t vswing : 1;
      uint8_t byte_10_bit_7 : 1;

      uint8_t byte_11;
      uint8_t byte_12;
      uint8_t byte_13;
      uint8_t byte_14;
      uint8_t byte_15;
      uint8_t byte_16;
      uint8_t byte_17;
      uint8_t byte_18;
      uint8_t byte_19;

      uint8_t byte_20;
      uint8_t byte_21;
      uint8_t byte_22;
      uint8_t byte_23;
      uint8_t byte_24;
      uint8_t byte_25;
      uint8_t byte_26;
      uint8_t byte_27;
      uint8_t byte_28;
      uint8_t byte_29;

      uint8_t byte_30;
      uint8_t byte_31;
      uint8_t byte_32;
      
      uint8_t byte_33_bit_0_6 : 7;
      uint8_t mute : 1;

      uint8_t byte_34;
      uint8_t byte_35;
      uint8_t byte_36;
      uint8_t byte_37;
      uint8_t byte_38;
      uint8_t byte_39;

      uint8_t byte_40;
      uint8_t byte_41;
      uint8_t byte_42;
      uint8_t byte_43;
      uint8_t byte_44;
      uint8_t volt45;
      uint8_t byte_46;
      uint8_t byte_47;
      uint8_t byte_48;
      uint8_t byte_49;

      uint8_t byte_50;

      uint8_t vswing_fix : 3;
      uint8_t vswing_mv : 2;
      uint8_t byte_51_bit_5_7 : 3;

      uint8_t hswing_fix : 3;
      uint8_t hswing_mv : 3;
      uint8_t byte_52_bit_6_7 : 2;

      uint8_t byte_53;
      uint8_t byte_54;
      uint8_t byte_55;
      uint8_t byte_56;
      uint8_t byte_57;
      uint8_t byte_58;
      uint8_t byte_59;

      uint8_t xor_sum;
    } data;
    uint8_t raw[61];
  };

  union set_cmd_t {
    struct {
      uint8_t header;
      uint8_t byte_1;
      uint8_t byte_2;
      uint8_t type;
      uint8_t len;
      uint8_t byte_5;
      uint8_t byte_6;
      
      uint8_t byte_7_bit_0_1 : 2;
      uint8_t power : 1;
      uint8_t off_timer_en : 1;
      uint8_t on_timer_en : 1;
      uint8_t beep : 1;
      uint8_t disp : 1;
      uint8_t eco : 1;

      uint8_t mode : 4;
      uint8_t byte_8_bit_4_5 : 2;
      uint8_t turbo : 1;
      uint8_t mute : 1;

      uint8_t temp : 4;
      uint8_t byte_9_bit_4_7 : 4;

      uint8_t fan : 3;
      uint8_t vswing : 3;
      uint8_t byte_10_bit_6 : 1;
      uint8_t byte_10_bit_7 : 1;

      uint8_t byte_11_bit_0_2 : 3;
      uint8_t hswing : 1;
      uint8_t byte_11_bit_4_7 : 4;

      uint8_t byte_12;
      uint8_t byte_13;

      uint8_t byte_14_bit_0_2 : 3;
      uint8_t byte_14_bit_3 : 1;
      uint8_t byte_14_bit_4 : 1;
      uint8_t half_degree : 1;
      uint8_t byte_14_bit_6_7 : 2;

      uint8_t byte_15;
      uint8_t byte_16;
      uint8_t byte_17;
      uint8_t byte_18;
      uint8_t byte_19;

      uint8_t byte_20;
      uint8_t byte_21;
      uint8_t byte_22;
      uint8_t byte_23;
      uint8_t byte_24;
      uint8_t byte_25;
      uint8_t byte_26;
      uint8_t byte_27;
      uint8_t byte_28;
      uint8_t byte_29;

      uint8_t byte_30;
      uint8_t byte_31;
      uint8_t vswing_fix : 3;
      uint8_t vswing_mv : 2;
      uint8_t byte_32_bit_5_7 : 3;

      uint8_t hswing_fix : 3;
      uint8_t hswing_mv : 3;
      uint8_t byte_33_bit_6_7 : 2;

      uint8_t xor_sum;
    } data;
    uint8_t raw[35];
  };  

  bool ready_to_send_set_cmd_flag = false;

  uint8_t set_cmd_base[35] = {
    0xBB, 0x00, 0x01, 0x03, 0x1D, 0x00, 0x00, 0x64, 0x03, 0xF3,
    0x00, 0x00, 0x00, 0x00, 0x00, // bytes 10-14
    0x00, 0x00, 0x00, 0x00, 0x00, // bytes 15-19
    0x00, 0x00, 0x00, 0x00, 0x00, // bytes 20-24
    0x00, 0x00, 0x00, 0x00, 0x00, // bytes 25-29
    0x00, 0x00, 0x00, 0x00        // bytes 30-33 (34 is xor_sum)
  };

  get_cmd_resp_t m_get_cmd_resp = {0};
  set_cmd_t m_set_cmd = {0};

  // ... [Member functions for setting internal state] ...

  float last_commanded_target_temperature = 0.0f;

  // Member functions to update internal state
  void set_current_temperature(float current_temperature) {
    if (this->current_temperature == current_temperature) return;
    this->is_changed = true;
    this->current_temperature = current_temperature;
  }

  void set_custom_fan_mode(const std::string &fan_mode) {
    if (this->custom_fan_mode == fan_mode) return;
    this->is_changed = true;
    this->custom_fan_mode = fan_mode;
  }

  void set_mode(esphome::climate::ClimateMode mode) {
    if (this->mode == mode) return;
    this->is_changed = true;
    this->mode = mode;
  }

  void set_swing_mode(esphome::climate::ClimateSwingMode swing_mode) {
    if (this->swing_mode == swing_mode) return;
    this->is_changed = true;
    this->swing_mode = swing_mode;
  }

  std::string hswing_pos = "";
  std::string vswing_pos = "";

  void set_hswing_pos(const std::string &hswing_pos) {
    if (this->hswing_pos == hswing_pos) return;
    id(hswing).publish_state(hswing_pos);
    this->hswing_pos = hswing_pos;
  }

  void set_vswing_pos(const std::string &vswing_pos) {
    if (this->vswing_pos == vswing_pos) return;
    id(vswing).publish_state(vswing_pos);
    this->vswing_pos = vswing_pos;
  }

  void set_target_temperature(float target_temperature) {
    if (this->target_temperature == target_temperature) return;
    this->is_changed = true;
    this->target_temperature = target_temperature;
  }

  // build_set_cmd() assembles the command packet.
  // (Note: We removed any forced resetting of half_degree here.)
  void build_set_cmd(get_cmd_resp_t *get_cmd_resp) {
    memcpy(m_set_cmd.raw, set_cmd_base, sizeof(m_set_cmd.raw));

    m_set_cmd.data.power = get_cmd_resp->data.power;
    m_set_cmd.data.off_timer_en = 0;
    m_set_cmd.data.on_timer_en = 0;
    m_set_cmd.data.beep = 0;
    m_set_cmd.data.disp = 0;
    m_set_cmd.data.eco = 0;

    switch (get_cmd_resp->data.mode) {
      case 0x01:
        m_set_cmd.data.mode = 0x03;
        break;
      case 0x03:
        m_set_cmd.data.mode = 0x02;
        break;
      case 0x02:
        m_set_cmd.data.mode = 0x07;
        break;
      case 0x04:
        m_set_cmd.data.mode = 0x01;
        break;
      case 0x05:
        m_set_cmd.data.mode = 0x08;
        break;
    }

    m_set_cmd.data.turbo = get_cmd_resp->data.turbo;
    m_set_cmd.data.mute = get_cmd_resp->data.mute;
    m_set_cmd.data.temp = 15 - get_cmd_resp->data.temp;

    switch (get_cmd_resp->data.fan) {
      case 0x00:
        m_set_cmd.data.fan = 0x00;
        break;
      case 0x01:
        m_set_cmd.data.fan = 0x02;
        break;
      case 0x04:
        m_set_cmd.data.fan = 0x06;
        break;
      case 0x02:
        m_set_cmd.data.fan = 0x03;
        break;
      case 0x05:
        m_set_cmd.data.fan = 0x07;
        break;
      case 0x03:
        m_set_cmd.data.fan = 0x05;
        break;
    }

    if (get_cmd_resp->data.vswing_mv) {
      m_set_cmd.data.vswing = 0x07;
      m_set_cmd.data.vswing_fix = 0;
      m_set_cmd.data.vswing_mv = get_cmd_resp->data.vswing_mv;
    } else if (get_cmd_resp->data.vswing_fix) {
      m_set_cmd.data.vswing = 0;
      m_set_cmd.data.vswing_fix = get_cmd_resp->data.vswing_fix;
      m_set_cmd.data.vswing_mv = 0;
    }

    if (get_cmd_resp->data.hswing_mv) {
      m_set_cmd.data.hswing = 0x01;
      m_set_cmd.data.hswing_fix = 0;
      m_set_cmd.data.hswing_mv = get_cmd_resp->data.hswing_mv;
    } else if (get_cmd_resp->data.hswing_fix) {
      m_set_cmd.data.hswing = 0;
      m_set_cmd.data.hswing_fix = get_cmd_resp->data.hswing_fix;
      m_set_cmd.data.hswing_mv = 0;
    }

    // Do not reset half_degree here; it will be applied externally.
    // m_set_cmd.data.half_degree = 0;

    // Calculate checksum
    m_set_cmd.data.xor_sum = 0;
    for (int i = 0; i < sizeof(m_set_cmd.raw) - 1; i++) {
      m_set_cmd.data.xor_sum ^= m_set_cmd.raw[i];
    }
  }

  void setup() override {
    set_update_interval(1000);
  }

  void control_vertical_swing(const std::string &swing_mode) {
    ESP_LOGD("custom", "Vertical swing set to: %s", swing_mode.c_str());

    get_cmd_resp_t get_cmd_resp = {0};
    memcpy(get_cmd_resp.raw, m_get_cmd_resp.raw, sizeof(get_cmd_resp.raw));

    get_cmd_resp.data.vswing_mv = 0;
    get_cmd_resp.data.vswing_fix = 0;

    if (swing_mode == "Move full") get_cmd_resp.data.vswing_mv = 0x01;
    else if (swing_mode == "Move upper")  get_cmd_resp.data.vswing_mv = 0x02;
    else if (swing_mode == "Move lower")  get_cmd_resp.data.vswing_mv = 0x03;
    else if (swing_mode == "Fix top") get_cmd_resp.data.vswing_fix = 0x01;
    else if (swing_mode == "Fix upper") get_cmd_resp.data.vswing_fix = 0x02;
    else if (swing_mode == "Fix mid") get_cmd_resp.data.vswing_fix = 0x03;
    else if (swing_mode == "Fix lower") get_cmd_resp.data.vswing_fix = 0x04;
    else if (swing_mode == "Fix bottom") get_cmd_resp.data.vswing_fix = 0x05;

  if (get_cmd_resp.data.vswing_mv) {
    get_cmd_resp.data.vswing = 0x01;
  } else {
    get_cmd_resp.data.vswing = 0;
  }

  // 🔧 Force fixed position behavior (ensure motion is stopped cleanly)
  if (get_cmd_resp.data.vswing_fix != 0) {
    get_cmd_resp.data.vswing_mv = 0;
    get_cmd_resp.data.vswing = 0;
  }

  ESP_LOGI("custom", "Sending VSWING: fix=%02X, mv=%02X, vswing=%02X",
    get_cmd_resp.data.vswing_fix,
    get_cmd_resp.data.vswing_mv,
    get_cmd_resp.data.vswing);

build_set_cmd(&get_cmd_resp);
ready_to_send_set_cmd_flag = true;

// Sync climate swing mode with this new vertical choice.
// Example logic: if "Move full" => vertical swing; if "Fix ..." => swing off
if (swing_mode == "Move full") {
  this->set_swing_mode(climate::CLIMATE_SWING_VERTICAL);
} else if (swing_mode.rfind("Fix", 0) == 0) {
  // If it's "Fix top", "Fix mid", "Fix bottom", etc., treat vertical as off.
  // If you want more advanced logic, you can check if horizontal is moving.
  this->set_swing_mode(climate::CLIMATE_SWING_OFF);
}

// Mark climate as changed and publish so HA sees the new state right away.
this->is_changed = true;
this->publish_state();



  }

  void control_horizontal_swing(const std::string &swing_mode) {
    ESP_LOGD("custom", "Horizontal swing set to: %s", swing_mode.c_str());

    get_cmd_resp_t get_cmd_resp = {0};
    memcpy(get_cmd_resp.raw, m_get_cmd_resp.raw, sizeof(get_cmd_resp.raw));

    get_cmd_resp.data.hswing_mv = 0;
    get_cmd_resp.data.hswing_fix = 0;

    if (swing_mode == "Move full") get_cmd_resp.data.hswing_mv = 0x01;
    else if (swing_mode == "Move left") get_cmd_resp.data.hswing_mv = 0x02;
    else if (swing_mode == "Move mid") get_cmd_resp.data.hswing_mv = 0x03;
    else if (swing_mode == "Move right") get_cmd_resp.data.hswing_mv = 0x04;
    else if (swing_mode == "Fix left") get_cmd_resp.data.hswing_fix = 0x01;
    else if (swing_mode == "Fix mid left") get_cmd_resp.data.hswing_fix = 0x02;
    else if (swing_mode == "Fix mid") get_cmd_resp.data.hswing_fix = 0x03;
    else if (swing_mode == "Fix mid right") get_cmd_resp.data.hswing_fix = 0x04;
    else if (swing_mode == "Fix right") get_cmd_resp.data.hswing_fix = 0x05;

    if (get_cmd_resp.data.hswing_mv) get_cmd_resp.data.hswing = 0x01;
    else get_cmd_resp.data.hswing = 0;

    build_set_cmd(&get_cmd_resp);
    ready_to_send_set_cmd_flag = true;
  }

  void control(const ClimateCall &call) override {
    if (call.get_mode().has_value()) {
      ClimateMode climate_mode = *call.get_mode();

      get_cmd_resp_t get_cmd_resp = {0};
      memcpy(get_cmd_resp.raw, m_get_cmd_resp.raw, sizeof(get_cmd_resp.raw));

      if (climate_mode == climate::CLIMATE_MODE_OFF) {
        get_cmd_resp.data.power = 0x00;
      } else {
        get_cmd_resp.data.power = 0x01;
        switch (climate_mode) {
          case climate::CLIMATE_MODE_COOL:
            get_cmd_resp.data.mode = 0x01;
            break;
          case climate::CLIMATE_MODE_DRY:
            get_cmd_resp.data.mode = 0x03;
            break;
          case climate::CLIMATE_MODE_FAN_ONLY:
            get_cmd_resp.data.mode = 0x02;
            break;
          case climate::CLIMATE_MODE_HEAT:
          case climate::CLIMATE_MODE_HEAT_COOL:
            get_cmd_resp.data.mode = 0x04;
            break;
          case climate::CLIMATE_MODE_AUTO:
            get_cmd_resp.data.mode = 0x05;
            break;
          default:
            break;
        }
      }

      build_set_cmd(&get_cmd_resp);
      ready_to_send_set_cmd_flag = true;
    }
    if (call.get_target_temperature().has_value()) {
   float ha_temp = *call.get_target_temperature();
   last_commanded_target_temperature = ha_temp;
  get_cmd_resp_t get_cmd_resp = {0};
  memcpy(get_cmd_resp.raw, m_get_cmd_resp.raw, sizeof(get_cmd_resp.raw));

  // Structure for temperature mapping range.
  struct TempMappingRange {
    float min;         // Inclusive lower bound (°C)
    float max;         // Exclusive upper bound (°C)
    uint8_t temp_val;  // Command temperature byte = (Whole°C - 16)
    bool half;         // Half-degree flag (true if .5°C)
  };

  // Lookup table for Fahrenheit 65°F (18.5°C) to 79°F (26.0°C).
  const TempMappingRange temp_map[] = {
  { -INFINITY, 18.75f, 2, true},    // 65°F => 18.5°C
  { 18.75f, 19.25f, 3, false},       // 66°F => 19.0°C
  { 19.25f, 19.75f, 3, true},        // 67°F => 19.5°C
  { 19.75f, 20.25f, 4, false},       // 68°F => 20.0°C
  { 20.25f, 20.75f, 4, true},        // 69°F => 20.5°C
  { 20.75f, 21.25f, 5, false},       // 70°F => 21.0°C
  { 21.25f, 21.75f, 5, true},        // 71°F => 21.5°C
  { 21.75f, 22.25f, 6, false},       // 72°F => 22.0°C
  { 22.25f, 22.75f, 6, true},        // 73°F => 22.5°C
  { 22.75f, 23.25f, 7, false},       // 74°F => 23.0°C
  { 23.25f, 23.75f, 7, true},        // 75°F => 23.5°C
  { 23.75f, 24.25f, 8, false},       // 76°F => 24.0°C
  { 24.25f, 24.75f, 8, true},        // 77°F => 24.5°C
  { 24.75f, 25.75f, 9, true},       // 78°F => 25.5°C
  { 25.75f, 26.25, 10, false},      // 79°F => 26.0°C
  { 26.25f, INFINITY, 10, true}      // 80°F => 26.5°C  
};


  bool found = false;
  bool half_flag = false;  // Local variable to hold the half flag.
  for (const auto &entry : temp_map) {
    if (ha_temp >= entry.min && ha_temp < entry.max) {
      get_cmd_resp.data.temp = entry.temp_val; // Set temperature byte.
      half_flag = entry.half;                   // Save the half flag.
      found = true;
      break;
    }
  }
  
  if (!found) {
    ESP_LOGW("TCL", "Temperature %.2f°C not found in mapping table", ha_temp);
    // Optionally clamp or default the value here.
  }

  // Build the command packet.
  build_set_cmd(&get_cmd_resp);

  // Reapply the half flag after build_set_cmd() in case it was overwritten.
  m_set_cmd.data.half_degree = half_flag ? 1 : 0;
  ESP_LOGI("TCL", "[DEBUG] Half flag re-applied as: %d", m_set_cmd.data.half_degree);

  // Recalculate checksum.
  m_set_cmd.data.xor_sum = 0;
  for (int i = 0; i < sizeof(m_set_cmd.raw) - 1; i++) {
    m_set_cmd.data.xor_sum ^= m_set_cmd.raw[i];
  }

  ESP_LOGI("TCL", "Target temperature set to: %.2f°C (temp byte: %d, half flag: %d)",
           ha_temp, get_cmd_resp.data.temp, m_set_cmd.data.half_degree);

  // Compute the commanded target temperature using our lookup:
  float commanded_target = (get_cmd_resp.data.temp + 16) + (m_set_cmd.data.half_degree ? 0.5f : 0.0f);
  // Save it so the UI can display what we intended.
  last_commanded_target_temperature = commanded_target;
  ready_to_send_set_cmd_flag = true;
}

    if (call.get_custom_fan_mode().has_value()) {
      std::string fan_mode = *call.get_custom_fan_mode();

      get_cmd_resp_t get_cmd_resp = {0};
      memcpy(get_cmd_resp.raw, m_get_cmd_resp.raw, sizeof(get_cmd_resp.raw));

      get_cmd_resp.data.turbo = 0x00;
      get_cmd_resp.data.mute = 0x00;
      if (fan_mode == "Turbo") { 
        get_cmd_resp.data.fan = 0x03;
        get_cmd_resp.data.turbo = 0x01;
      } else if (fan_mode == "Mute") {
        get_cmd_resp.data.fan = 0x01;
        get_cmd_resp.data.mute = 0x01;
      } else if (fan_mode == "Automatic") get_cmd_resp.data.fan = 0x00;
      else if (fan_mode == "1") get_cmd_resp.data.fan = 0x01;
      else if (fan_mode == "2") get_cmd_resp.data.fan = 0x04;
      else if (fan_mode == "3") get_cmd_resp.data.fan = 0x02;
      else if (fan_mode == "4") get_cmd_resp.data.fan = 0x05;
      else if (fan_mode == "5") get_cmd_resp.data.fan = 0x03;

      build_set_cmd(&get_cmd_resp);
      ready_to_send_set_cmd_flag = true;
    }
// Handle the main climate card's swing mode
if (call.get_swing_mode().has_value()) {
  auto new_swing = *call.get_swing_mode();

  // We'll call your existing control_{vertical,horizontal}_swing() functions
  // then explicitly update your custom drop-down entities so they remain in sync.
  switch (new_swing) {
    case climate::CLIMATE_SWING_OFF:
      // vertical => "Fix top" (or whichever stable position you like)
      control_vertical_swing("Fix top");
      // horizontal => "Fix mid"
      control_horizontal_swing("Fix mid");

      // Update your custom drop-downs so they show the new positions
      id(vswing).publish_state("Fix top");
      id(hswing).publish_state("Fix mid");
      break;

    case climate::CLIMATE_SWING_VERTICAL:
      // vertical => "Move full"
      control_vertical_swing("Move full");
      // horizontal => forced off/fixed
      control_horizontal_swing("Fix mid");

      id(vswing).publish_state("Move full");
      id(hswing).publish_state("Fix mid");
      break;

    case climate::CLIMATE_SWING_HORIZONTAL:
      // vertical => forced off/fixed
      control_vertical_swing("Fix top");
      // horizontal => "Move full"
      control_horizontal_swing("Move full");

      id(vswing).publish_state("Fix top");
      id(hswing).publish_state("Move full");
      break;

    case climate::CLIMATE_SWING_BOTH:
      // vertical => "Move full"
      control_vertical_swing("Move full");
      // horizontal => "Move full"
      control_horizontal_swing("Move full");

      id(vswing).publish_state("Move full");
      id(hswing).publish_state("Move full");
      break;

    default:
      break;
  }
}


  }

  ClimateTraits traits() override {
    auto traits = climate::ClimateTraits();
    traits.set_supports_current_temperature(true);
    traits.set_supported_modes({
      climate::CLIMATE_MODE_OFF,
      climate::CLIMATE_MODE_COOL,
      climate::CLIMATE_MODE_HEAT,
      climate::CLIMATE_MODE_FAN_ONLY,
      climate::CLIMATE_MODE_DRY,
      climate::CLIMATE_MODE_AUTO
    });
    traits.set_supported_custom_fan_modes({
      "Turbo", "Mute", "Automatic", "1", "2", "3", "4", "5"
    });
    traits.set_supported_swing_modes({
      climate::CLIMATE_SWING_OFF,
      climate::CLIMATE_SWING_BOTH,
      climate::CLIMATE_SWING_VERTICAL,
      climate::CLIMATE_SWING_HORIZONTAL
    });
    traits.set_visual_min_temperature(16.5);
    traits.set_visual_max_temperature(26.5);
    traits.set_visual_target_temperature_step(1);
    return traits;
  }

  void update() override {
    uint8_t req_cmd[] = {0xBB, 0x00, 0x01, 0x04, 0x02, 0x01, 0x00, 0xBD};

    if (ready_to_send_set_cmd_flag) {
      ESP_LOGW("TCL", "Sending data");
      ready_to_send_set_cmd_flag = false;
      write_array(m_set_cmd.raw, sizeof(m_set_cmd.raw));
    } else {
      write_array(req_cmd, sizeof(req_cmd));
    }
  }

  int read_data_line(int readch, uint8_t *buffer, int len) {
    static int pos = 0;
    static bool wait_len = false;
    static int skipch = 0;

    if (readch >= 0) {
      if (readch == 0xBB && skipch == 0 && !wait_len) {
        pos = 0;
        skipch = 3; // wait char with len
        wait_len = true;
        if (pos < len - 1) buffer[pos++] = readch;
      } else if (skipch == 0 && wait_len) {
        if (pos < len - 1) buffer[pos++] = readch;
        skipch = readch + 1; // +1 control sum
        ESP_LOGI("TCL", "len: %d", readch);
        wait_len = false;
      } else if (skipch > 0) {
        if (pos < len - 1) buffer[pos++] = readch;
        if (--skipch == 0 && !wait_len) return pos;
      }
    }
    return -1;
  }

  bool is_valid_xor(uint8_t *buffer, int len) {
    uint8_t xor_byte = 0;
    for (int i = 0; i < len - 1; i++) xor_byte ^= buffer[i];
    if (xor_byte == buffer[len - 1]) return true;
    else {
      ESP_LOGW("TCL", "Invalid XOR checksum. Received: %02X, Calculated: %02X", buffer[len - 1], xor_byte);
      return false;
    }
  }

  void print_hex_str(uint8_t *buffer, int len) {
    char str[250] = {0};
    char *pstr = str;
    if (len * 3 > sizeof(str)) {
      ESP_LOGE("TCL", "Too long byte data");
      return;
    }

    for (int i = 0; i < len; i++) {
      pstr += sprintf(pstr, "%02X ", buffer[i]);
    }

    ESP_LOGI("TCL", "Received: %s", str);
  }

  void loop() override {
    const int max_line_length = 100;
    static uint8_t buffer[max_line_length];
    
    while (available()) {
      int len = read_data_line(read(), buffer, max_line_length);
      if(len == sizeof(m_get_cmd_resp) && buffer[3] == 0x04) {
        memcpy(m_get_cmd_resp.raw, buffer, len);
   //     print_hex_str(buffer, len);
        if (is_valid_xor(buffer, len)) {
  //          print_hex_str(buffer, len);
          float curr_temp = (((buffer[17] << 8) | buffer[18]) / 374.0 - 32.0) / 1.8;
          this->is_changed = false;

          // Update climate mode
          if (m_get_cmd_resp.data.power == 0x00) this->set_mode(climate::CLIMATE_MODE_OFF);
          else if (m_get_cmd_resp.data.mode == 0x01) this->set_mode(climate::CLIMATE_MODE_COOL);
          else if (m_get_cmd_resp.data.mode == 0x03) this->set_mode(climate::CLIMATE_MODE_DRY);
          else if (m_get_cmd_resp.data.mode == 0x02) this->set_mode(climate::CLIMATE_MODE_FAN_ONLY);
          else if (m_get_cmd_resp.data.mode == 0x04) this->set_mode(climate::CLIMATE_MODE_HEAT);
          else if (m_get_cmd_resp.data.mode == 0x05) this->set_mode(climate::CLIMATE_MODE_AUTO);

          // Update fan mode
          if (m_get_cmd_resp.data.turbo) this->set_custom_fan_mode("Turbo");
          else if (m_get_cmd_resp.data.mute) this->set_custom_fan_mode("Mute");
          else if (m_get_cmd_resp.data.fan == 0x00) this->set_custom_fan_mode("Automatic");
          else if (m_get_cmd_resp.data.fan == 0x01) this->set_custom_fan_mode("1");
          else if (m_get_cmd_resp.data.fan == 0x04) this->set_custom_fan_mode("2");
          else if (m_get_cmd_resp.data.fan == 0x02) this->set_custom_fan_mode("3");
          else if (m_get_cmd_resp.data.fan == 0x05) this->set_custom_fan_mode("4");
          else if (m_get_cmd_resp.data.fan == 0x03) this->set_custom_fan_mode("5");

          // Update swing mode
          if (m_get_cmd_resp.data.hswing && m_get_cmd_resp.data.vswing) this->set_swing_mode(climate::CLIMATE_SWING_BOTH);
          else if (!m_get_cmd_resp.data.hswing && !m_get_cmd_resp.data.vswing) this->set_swing_mode(climate::CLIMATE_SWING_OFF);
          else if (m_get_cmd_resp.data.vswing) this->set_swing_mode(climate::CLIMATE_SWING_VERTICAL);
          else if (m_get_cmd_resp.data.hswing) this->set_swing_mode(climate::CLIMATE_SWING_HORIZONTAL);

          // Update swing positions
          if (m_get_cmd_resp.data.vswing_mv == 0x01) set_vswing_pos("Move full");
          else if (m_get_cmd_resp.data.vswing_mv == 0x02) set_vswing_pos("Move upper");
          else if (m_get_cmd_resp.data.vswing_mv == 0x03) set_vswing_pos("Move lower");
          else if (m_get_cmd_resp.data.vswing_fix == 0x01) set_vswing_pos("Fix top");
          else if (m_get_cmd_resp.data.vswing_fix == 0x02) set_vswing_pos("Fix upper");
          else if (m_get_cmd_resp.data.vswing_fix == 0x03) set_vswing_pos("Fix mid");
          else if (m_get_cmd_resp.data.vswing_fix == 0x04) set_vswing_pos("Fix lower");
          else if (m_get_cmd_resp.data.vswing_fix == 0x05) set_vswing_pos("Fix bottom");
          else set_vswing_pos("Last position");

          if (m_get_cmd_resp.data.hswing_mv == 0x01) set_hswing_pos("Move full");
          else if (m_get_cmd_resp.data.hswing_mv == 0x02) set_hswing_pos("Move left");
          else if (m_get_cmd_resp.data.hswing_mv == 0x03) set_hswing_pos("Move mid");
          else if (m_get_cmd_resp.data.hswing_mv == 0x04) set_hswing_pos("Move right");
          else if (m_get_cmd_resp.data.hswing_fix == 0x01) set_hswing_pos("Fix left");
          else if (m_get_cmd_resp.data.hswing_fix == 0x02) set_hswing_pos("Fix mid left");
          else if (m_get_cmd_resp.data.hswing_fix == 0x03) set_hswing_pos("Fix mid");
          else if (m_get_cmd_resp.data.hswing_fix == 0x04) set_hswing_pos("Fix mid right");
          else if (m_get_cmd_resp.data.hswing_fix == 0x05) set_hswing_pos("Fix right");
          else set_hswing_pos("Last position");

          ESP_LOGI("TCL", "fan %02X", m_get_cmd_resp.data.fan);
          ESP_LOGI("TCL", "mode %02X", m_get_cmd_resp.data.mode);

          ESP_LOGI("TCL", "power: %d", m_get_cmd_resp.data.power);
          ESP_LOGI("TCL", "display: %d", m_get_cmd_resp.data.disp);
          ESP_LOGI("TCL", "eco: %d", m_get_cmd_resp.data.eco);
          ESP_LOGI("TCL", "turbo: %d", m_get_cmd_resp.data.turbo);
          ESP_LOGI("TCL", "mute: %d", m_get_cmd_resp.data.mute);
           
          ESP_LOGI("TCL", "current_temp: %.2f°C", curr_temp);
		      ESP_LOGI("TCL", "target_temp: %.1f°C", (m_get_cmd_resp.data.temp + 16) + (m_set_cmd.data.half_degree ? 0.5f : 0.0f));

          // Update target and current temperatures
          this->set_target_temperature(last_commanded_target_temperature);
          this->set_current_temperature(curr_temp);

          if (this->is_changed)
            this->publish_state();
        }
      }
    }
  }
};
