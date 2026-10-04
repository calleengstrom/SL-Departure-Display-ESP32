#include "../include/httpClient.h"
#include "esp_crt_bundle.h"
esp_err_t httpClient::http_event_handler(esp_http_client_event_t *evt)
{
    switch (evt->event_id)
    {
    case HTTP_EVENT_ON_DATA:
    {
        auto *clientObject =
            static_cast<httpClient *>(evt->user_data);

        clientObject->json_response.append(
            static_cast<char *>(evt->data),
            evt->data_len);

        break;
    }

    default:
        break;
    }

    return ESP_OK;
}

std::string httpClient::get_request(const std::string &api_endpoint)
{
    json_response.clear();

    esp_http_client_config_t config = {};

    

    config.url = api_endpoint.c_str();
    config.method = HTTP_METHOD_GET;
    config.timeout_ms = 5000;
    config.event_handler = http_event_handler;
    config.user_data = this;

    config.crt_bundle_attach = esp_crt_bundle_attach;

    esp_http_client_handle_t client =
        esp_http_client_init(&config);

    esp_http_client_set_header(
        client,
        "Accept",
        "application/json");

    esp_err_t err =
        esp_http_client_perform(client);

    if (err != ESP_OK)
    {
        esp_http_client_cleanup(client);
        return "";
    }

    esp_http_client_cleanup(client);

    return json_response;
}

httpClient::httpClient() {}