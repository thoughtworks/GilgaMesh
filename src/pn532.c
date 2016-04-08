#include "pn532.h"

bool first_response_received = false;

typedef enum {
    ACK_START,
    second_byte,
    third_byte,
    fourth_byte,
    fifth_byte,
    sixth_byte,
    ack_error,
    ack_processed
} ack_state;

ack_state current_ack_state = ACK_START;

void process_ack(uint8_t rx_byte) {
    switch (current_ack_state) {
        case ACK_START:
            if (rx_byte == 0) current_ack_state = second_byte;
            else current_ack_state = ack_error;
            break;

        case second_byte:
            if (rx_byte == 0) current_ack_state = third_byte;
            else current_ack_state = ack_error;
            break;

        case third_byte:
            if (rx_byte == 255) current_ack_state = fourth_byte;
            else current_ack_state = ack_error;
            break;

        case fourth_byte:
            if (rx_byte == 0) current_ack_state = fifth_byte;
            else current_ack_state = ack_error;
            break;

        case fifth_byte:
            if (rx_byte == 255) current_ack_state = sixth_byte;
            else current_ack_state = ack_error;
            break;

        case sixth_byte:
            if (rx_byte == 0) current_ack_state = ack_processed;
            else current_ack_state = ack_error;
            break;
    }
}

typedef enum {
    RESPONSE_NEEDS_PROCESSING,
    PREAMBLE,
    START1,
    START2,
    NO_TAG_EXISTS,
    TAG_EXISTS,
    NOT_EXPECTED_RESPONSE
} response_state;

response_state current_response_state = RESPONSE_NEEDS_PROCESSING;

void process_response(uint8_t rx_byte) {
    switch (current_response_state) {
        case RESPONSE_NEEDS_PROCESSING:
            if (rx_byte == 0) current_response_state = PREAMBLE;
            else current_response_state = NOT_EXPECTED_RESPONSE;
            break;

        case PREAMBLE:
            if (rx_byte == 0) current_response_state = START1;
            else current_response_state = NOT_EXPECTED_RESPONSE;
            break;

        case START1:
            if (rx_byte == 255) current_response_state = START2;
            else current_response_state = NOT_EXPECTED_RESPONSE;
            break;

        case START2:
            if (rx_byte == 3) {
                current_response_state = NO_TAG_EXISTS;
            }
            else if (rx_byte == 15) current_response_state = TAG_EXISTS;
            else current_response_state = NOT_EXPECTED_RESPONSE;
            break;
    }
}

typedef enum {
    START_PROCESSING_REST_OF_RESPONSE,
    LCS,
    TO_HOST,
    TAG_NUMBER,
    BAUDRATE,
    DATA2,
    DATA3,
    DATA4,
    DATA5,
    DATA6,
    DATA7,
    DATA8,
    DATA9,
    DATA10,
    DATA11,
    DATA12,
    DATA13,
    POSTAMBLE,
    REST_OF_RESPONSE_PROCESSED,
    NOT_EXPECTED_REST_OF_RESPONSE
} rest_of_response_state_t;

rest_of_response_state_t rest_of_response_state = START_PROCESSING_REST_OF_RESPONSE;

void process_rest_of_tag_exists_response(uint8_t rx_byte) {
    switch (rest_of_response_state) {
        case START_PROCESSING_REST_OF_RESPONSE:
            if (rx_byte == 241) rest_of_response_state = LCS;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

            // could be any number
        case LCS:
            if (rx_byte == 213) rest_of_response_state = TO_HOST;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case TO_HOST:
            if (rx_byte == 75) {
                rest_of_response_state = TAG_NUMBER;
            }
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case TAG_NUMBER:
            if (rx_byte == 1) {
                // there is one tag present
                rest_of_response_state = BAUDRATE;
            } else {
                // two tags at once is not supported
                rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            }
            break;

        case BAUDRATE:
            if (rx_byte == 1) {
                rest_of_response_state = DATA2;
            }
            else {
                rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            }
            break;

            // data bytes i don't care about
        case DATA2:
            if (rx_byte < 255) {
                rest_of_response_state = DATA3;
            }
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA3:
            //if (rx_byte == 68) {
            if (rx_byte < 255) {
                rest_of_response_state = DATA4;
            }
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

            // data byte i don't care about
        case DATA4:
            //if (rx_byte == 0) rest_of_response_state = DATA5;
            if (rx_byte < 255) {
                rest_of_response_state = DATA5;

            }
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA5:
            //if (rx_byte == 7) rest_of_response_state = DATA6;
            if (rx_byte < 255) rest_of_response_state = DATA6;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA6:
            if (rx_byte < 255) rest_of_response_state = DATA7;
                //if (rx_byte == 4) rest_of_response_state = DATA7;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA7:
            if (rx_byte < 255) rest_of_response_state = DATA8;
                //if (rx_byte == 160) rest_of_response_state = DATA8;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA8:
            if (rx_byte < 255) rest_of_response_state = DATA9;
                //if (rx_byte == 53) rest_of_response_state = DATA9;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA9:
            if (rx_byte < 255) rest_of_response_state = DATA10;
                //if (rx_byte == 178) rest_of_response_state = DATA10;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA10:
            if (rx_byte < 255) rest_of_response_state = DATA11;
                //if (rx_byte == 188) rest_of_response_state = DATA11;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA11:
            if (rx_byte < 255) rest_of_response_state = DATA12;
                //if (rx_byte == 43) rest_of_response_state = DATA12;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA12:
            if (rx_byte < 255) rest_of_response_state = DATA13;
                //if (rx_byte == 128) rest_of_response_state = DATA13;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case DATA13:
            if (rx_byte < 255) rest_of_response_state = POSTAMBLE;
                //if (rx_byte == 161) rest_of_response_state = POSTAMBLE;
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;

        case POSTAMBLE:
            //if (rx_byte == 0) {
            if (rx_byte < 255) {
                rest_of_response_state = REST_OF_RESPONSE_PROCESSED;
            }
            else rest_of_response_state = NOT_EXPECTED_REST_OF_RESPONSE;
            break;
            // 00 00 FF 0F F1 D5 4B 01 01 00 44 00 07 04 A0 35 B2 BC 2B 80 A1 00 - tag exists
    }
}

typedef enum {
    start_grabbing,
    end_grabbing
} data_grab_state_t;

data_grab_state_t grab_state = start_grabbing;

int max_remaining_byte_size_in_a_frame = 16; // check that this shouldn't be 15 because we eat 1 byte each data command :(
#define four_frame_data_dump_size 64
static uint8_t data_dump1[four_frame_data_dump_size] = {0};
int data_byte_count = 0;
int dump_index = 0;

void grab_frame_bytes_into_mass_dump(uint8_t rx_byte) {
    switch (grab_state) {
        case start_grabbing:
            if (data_byte_count < max_remaining_byte_size_in_a_frame) {
                data_dump1[dump_index] = rx_byte;
                data_byte_count++;
                dump_index++;
                grab_state = start_grabbing;
            } else {
                grab_state = end_grabbing;
                data_byte_count = 0;
            }
            break;
    }
}
/* UNDER TEST */

bool id_exists_in_response(uint8_t *response, size_t response_length) {
    char *first_occurrence_of_equals = memchr(response, '=', response_length);

    if (!first_occurrence_of_equals) return false;

    int expected_digits_in_id = 4;
    for (int i = 1; i <= expected_digits_in_id; i++) {
        if (!isdigit(first_occurrence_of_equals[i])) return false;
    }

    return true;
}

short get_id(uint8_t *response, size_t response_length) {
    char *first_occurrence_of_equals = memchr(response, '=', response_length);

    int first_digit = first_occurrence_of_equals[1] - '0';
    int second_digit = first_occurrence_of_equals[2] - '0';
    int third_digit = first_occurrence_of_equals[3] - '0';
    int fourth_digit = first_occurrence_of_equals[4] - '0';

    short full_id = (short) (first_digit * 1000 + second_digit * 100 + third_digit * 10 + fourth_digit);

    return full_id;
}

/* ******** */

typedef enum {
    SETUP,
    NEEDS_RETRY,
    ERROR,
    TAG_PRESENCE_UNKNOWN, // check if tag exists
    IN_LIST_ACK_PROCESSING,
    PROCESSING_REST_OF_RESPONSE,
    SECOND_READ_ACK,
    THIRD_READ_ACK,
    GET_PAID_1,
    GET_PAID_2,
    GET_PAID_3,
    GET_PAID_4,
    FOURTH_READ_ACK,
    FIFTH_READ_ACK,
    EAT_FIRST_DATA_COMMAND,
    EAT_SECOND_DATA_COMMAND,
    EAT_THIRD_DATA_COMMAND,
    EAT_FOURTH_DATA_COMMAND,
    ID_TAKEN
} nfc_state_t;

nfc_state_t current_nfc_state = SETUP;

void nfcEventHandler(uint8_t rx_byte) {
    switch (current_nfc_state) {
        case ID_TAKEN:
            // too bad ass to keep going
            break;

        case NEEDS_RETRY:
            // do nothing until we need to search for tag again
            break;

        case ERROR:
            //node->PutInRetryStorage(8888);
            // STORAGE
            break;

        case SETUP:
            if (rx_byte == 255) first_response_received = true;
            break;

        case TAG_PRESENCE_UNKNOWN:
            // Handle Ack
            if (current_ack_state != ack_processed) process_ack(rx_byte);
            if (current_ack_state == ack_error) {
                current_nfc_state = NEEDS_RETRY;
                led_green_on();
            }
            if (current_ack_state == ack_processed) current_nfc_state = IN_LIST_ACK_PROCESSING;
            break;

        case IN_LIST_ACK_PROCESSING:
            if (current_response_state != NO_TAG_EXISTS || current_response_state != TAG_EXISTS)
                process_response(rx_byte);
            if (current_response_state == NOT_EXPECTED_RESPONSE) current_nfc_state = NEEDS_RETRY;
            if (current_response_state == NO_TAG_EXISTS) current_nfc_state = NEEDS_RETRY;
            if (current_response_state == TAG_EXISTS) current_nfc_state = PROCESSING_REST_OF_RESPONSE;
            break;

        case PROCESSING_REST_OF_RESPONSE:
            if (rest_of_response_state != REST_OF_RESPONSE_PROCESSED) process_rest_of_tag_exists_response(rx_byte);
            if (rest_of_response_state == NOT_EXPECTED_REST_OF_RESPONSE) current_nfc_state = NEEDS_RETRY;

            if (rest_of_response_state == REST_OF_RESPONSE_PROCESSED) {
                in_data_exchange('\x00', '\xBB');
                current_nfc_state = SECOND_READ_ACK;
            }
            break;

        case SECOND_READ_ACK:
            if (rx_byte == 65) {
                // \x41
                current_nfc_state = EAT_FIRST_DATA_COMMAND;
            }
            break;

        case EAT_FIRST_DATA_COMMAND:
            if (rx_byte == 0) current_nfc_state = GET_PAID_1;
            break;

        case GET_PAID_1:
            grab_state = start_grabbing;
            if (grab_state != end_grabbing) grab_frame_bytes_into_mass_dump(rx_byte);

            if (grab_state == end_grabbing) {
                in_data_exchange('\x04', '\xB7');
                current_nfc_state = THIRD_READ_ACK;
            }
            break;

        case THIRD_READ_ACK:
            if (rx_byte == 65) {
                // \x41
                current_nfc_state = EAT_SECOND_DATA_COMMAND;
            }
            break;

        case EAT_SECOND_DATA_COMMAND:
            if (rx_byte == 0) current_nfc_state = GET_PAID_2;
            break;

        case GET_PAID_2:
            grab_state = start_grabbing;
            if (grab_state != end_grabbing) grab_frame_bytes_into_mass_dump(rx_byte);

            if (grab_state == end_grabbing) {
                in_data_exchange('\x08', '\xB3');
                current_nfc_state = FOURTH_READ_ACK;
            }
            break;

        case FOURTH_READ_ACK:
            // \x41
            if (rx_byte == 65) {
                current_nfc_state = EAT_THIRD_DATA_COMMAND;
            }
            break;

        case EAT_THIRD_DATA_COMMAND:
            if (rx_byte == 0) current_nfc_state = GET_PAID_3;
            break;

        case GET_PAID_3:
            grab_state = start_grabbing;
            if (grab_state != end_grabbing) grab_frame_bytes_into_mass_dump(rx_byte);

            if (grab_state == end_grabbing) {
                in_data_exchange('\x0C', '\xAF');
                current_nfc_state = FIFTH_READ_ACK;
            }
            break;

        case FIFTH_READ_ACK:
            // \x41
            if (rx_byte == 65)
                current_nfc_state = GET_PAID_4;
            break;

        case EAT_FOURTH_DATA_COMMAND:
            if (rx_byte == 0) current_nfc_state = GET_PAID_4;
            break;

        case GET_PAID_4:
            grab_state = start_grabbing;
            if (grab_state != end_grabbing) grab_frame_bytes_into_mass_dump(rx_byte);

            if (grab_state == end_grabbing) {
                if (id_exists_in_response(data_dump1, four_frame_data_dump_size)) {
                    // node->PutInRetryStorage(get_id(data_dump1, four_frame_data_dump_size));

                    // node->FlashWhiteAndBuzz(3);
                    led_white_on();
                    // BUZZ!

                    current_nfc_state = ID_TAKEN;
                } else {
                    current_nfc_state = NEEDS_RETRY;
                }
            }
            break;
    }
}


void setup_state_machine() {
    current_response_state = RESPONSE_NEEDS_PROCESSING;
    rest_of_response_state = START_PROCESSING_REST_OF_RESPONSE;
    current_nfc_state = TAG_PRESENCE_UNKNOWN;
    current_ack_state = ACK_START;
    dump_index = 0;
    for (int i = 0; i < four_frame_data_dump_size; i++) {
        data_dump1[i] = 0;
    }
}

static uart_event_handler m_event_handler;

setup_state_t current_setup_state = DOWN;

void setup() {
    switch (current_setup_state) {
        case DOWN:
            wakeup();
            current_setup_state = NO_PARAM;
            break;

        case NO_PARAM:
            set_parameter();
            current_setup_state = NOT_WRITTEN_80;
            break;

        case NOT_WRITTEN_80:
            write_80();
            current_setup_state = RF_NOT_CONFIG;
            break;

        case RF_NOT_CONFIG:
            config_rf();
            current_setup_state = NOT_WRITTEN_40_10;
            break;

        case NOT_WRITTEN_40_10:
            write_40_10();
            current_setup_state = RF_MAX_NOT_CONFIG;
            break;

        case RF_MAX_NOT_CONFIG:
            config_rf_max();
            nrf_delay_us(1000);
            current_setup_state = SETUP_DONE;
            break;
    }
}

setup_state_t get_setup_state() {
    return current_setup_state;
}

void wakeup() {
    simple_uart_put('\x55');
    simple_uart_put('\x55');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\xFF');
    simple_uart_put('\x03');
    simple_uart_put('\xFD');
    simple_uart_put('\xD4');
    simple_uart_put('\x14');
    simple_uart_put('\x01');
    simple_uart_put('\x17');
    simple_uart_put('\x00');
}

void set_parameter() {
    send_preamble_and_start();
    simple_uart_put('\x03');
    simple_uart_put('\xFD');
    send_direction();
    simple_uart_put('\x12'); // set parameter command
    simple_uart_put('\x14');
    simple_uart_put('\x06');
    send_postamble();
}

void write_80() {
    send_preamble_and_start();
    simple_uart_put('\x08');
    simple_uart_put('\xF8');
    send_direction();
    simple_uart_put('\x08'); // write register
    simple_uart_put('\x63');
    simple_uart_put('\x02');
    simple_uart_put('\x80');
    simple_uart_put('\x63');
    simple_uart_put('\x03');
    simple_uart_put('\x80');
    simple_uart_put('\x59');
    send_postamble();
}

void config_rf() {
    send_preamble_and_start();
    simple_uart_put('\x04');
    simple_uart_put('\xFC');
    send_direction();
    simple_uart_put('\x32'); // rfconfiguration
    simple_uart_put('\x01');
    simple_uart_put('\x01');
    simple_uart_put('\xF8');
    send_postamble();
}

void write_40_10() {
    send_preamble_and_start();
    simple_uart_put('\x08');
    simple_uart_put('\xF8');
    send_direction();
    simple_uart_put('\x08'); // write register
    simple_uart_put('\x63');
    simple_uart_put('\x05');
    simple_uart_put('\x40');
    simple_uart_put('\x63');
    simple_uart_put('\x3C');
    simple_uart_put('\x10');
    simple_uart_put('\xCD');
    send_postamble();
}

void config_rf_max() {
    send_preamble_and_start();
    simple_uart_put('\x06');
    simple_uart_put('\xFA');
    send_direction();
    simple_uart_put('\x32'); // rf configuration
    simple_uart_put('\x05');
    simple_uart_put('\x00');
    simple_uart_put('\x01');
    simple_uart_put('\x02');
    simple_uart_put('\xF2');
    send_postamble();
}

void send_preamble_and_start() {
    simple_uart_put('\x00');
    simple_uart_put('\x00');
    simple_uart_put('\xFF');
}

void send_direction() {
    simple_uart_put('\xD4');
}

void send_postamble() {
    simple_uart_put('\x00');
}

const uint8_t LENGTH = '\x05';
const uint8_t LENGTH_CHECK_SUM = '\xFB';
const uint8_t COMMAND = '\x40';
const uint8_t LOGICAL_NUMBER = '\x01';
const uint8_t MIFARE_COMMAND = '\x30';

void in_data_exchange(uint8_t start_address, uint8_t dcs) {
    send_preamble_and_start();
    simple_uart_put(LENGTH);
    simple_uart_put(LENGTH_CHECK_SUM);
    send_direction();
    simple_uart_put(COMMAND);
    simple_uart_put(LOGICAL_NUMBER);
    simple_uart_put(MIFARE_COMMAND);
    simple_uart_put(start_address);
    simple_uart_put(dcs);
    send_postamble();
}

unsigned short in_list_passive_target() {
    send_preamble_and_start();
    simple_uart_put('\x04');
    simple_uart_put('\xFC');
    send_direction();
    simple_uart_put('\x4A');
    simple_uart_put('\x01');
    simple_uart_put('\x00');
    simple_uart_put('\xE1');
    send_postamble();
}

void powerdown() {
    send_preamble_and_start();
    simple_uart_put('\x03');
    simple_uart_put('\xFD');
    simple_uart_put('\xD4');
    simple_uart_put('\x16');
    simple_uart_put('\xF0');
    simple_uart_put('\x26');
    send_postamble();
}

void UART0_IRQHandler(void) {
    // Handle reception
    if ((NRF_UART0->EVENTS_RXDRDY != 0) && (NRF_UART0->INTENSET & UART_INTENSET_RXDRDY_Msk)) {
        // Clear UART RX event flag
        NRF_UART0->EVENTS_RXDRDY = 0;

        m_event_handler((uint8_t) NRF_UART0->RXD);
    }

    // Handle transmission.
    if ((NRF_UART0->EVENTS_TXDRDY != 0) && (NRF_UART0->INTENSET & UART_INTENSET_TXDRDY_Msk)) {
        // Clear UART TX event flag.
        NRF_UART0->EVENTS_TXDRDY = 0;
    }

    // Handle errors.
    if ((NRF_UART0->EVENTS_ERROR != 0) && (NRF_UART0->INTENSET & UART_INTENSET_ERROR_Msk)) {
        // Check SDK implementation of UART for an example of error handling
    }
}

void uart_115200_config(uint8_t txd_pin_number,
                        uint8_t rxd_pin_number,
                        uint8_t buzzer_pin_number,
                        uart_event_handler event_handler) {
    m_event_handler = event_handler;

    nrf_gpio_pin_clear(buzzer_pin_number);
    nrf_gpio_cfg_output(txd_pin_number);
    nrf_gpio_cfg_input(rxd_pin_number, NRF_GPIO_PIN_PULLUP);

    NRF_UART0->PSELTXD = txd_pin_number;
    NRF_UART0->PSELRXD = rxd_pin_number;
    NRF_UART0->BAUDRATE = (UART_BAUDRATE_BAUDRATE_Baud115200);

    // parity excluded
    //?? needed?
    NRF_UART0->CONFIG = (UART_CONFIG_PARITY_Excluded << UART_CONFIG_PARITY_Pos);

    // flow control disabled
    NRF_UART0->ENABLE = (UART_ENABLE_ENABLE_Enabled << UART_ENABLE_ENABLE_Pos);
    NRF_UART0->EVENTS_RXDRDY = 0;
    NRF_UART0->EVENTS_TXDRDY = 0;

    //?? needed?
    NRF_UART0->CONFIG &= ~(UART_CONFIG_HWFC_Enabled << UART_CONFIG_HWFC_Pos);
    NRF_UART0->TASKS_STARTTX = 1;
    NRF_UART0->TASKS_STARTRX = 1;

    // enable uart interrupt
    NRF_UART0->INTENCLR = 0xffffffffUL;
    NRF_UART0->INTENSET = (UART_INTENSET_RXDRDY_Set << UART_INTENSET_RXDRDY_Pos);

    NVIC_ClearPendingIRQ(UART0_IRQn);
    NVIC_SetPriority(UART0_IRQn, APP_IRQ_PRIORITY_LOW);
    NVIC_EnableIRQ(UART0_IRQn);
}

void initialize_uart() {
    uart_115200_config(19, 20, 6, nfcEventHandler);
}

//check error with below code to see if in error for logging here :D
//if (get_setup_state() == SETUP_DONE) {
//if (first_response_received)
//    current_nfc_state = TAG_PRESENCE_UNKNOWN;
//else {
//    led_red_on();
//}
//}

void manage_badge_reading() {
    if (get_setup_state() != SETUP_DONE) setup();
    if (get_setup_state() == SETUP_DONE) {
        setup_state_machine();
        in_list_passive_target();
    }
}
