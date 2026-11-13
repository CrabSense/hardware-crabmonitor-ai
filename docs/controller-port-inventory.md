# Controller port inventory v1

The NO2 firmware reports the four outputs already defined in `config/Pins.h`
as **declared**, not electrically detected. It never activates a GPIO for
discovery; the NO2 pump/valve drivers remain the only owners of those outputs.
Remote control is false for all NO2 outputs. This is not the four-SSR RAS
controller firmware.

The reporter starts from setup in a separate FreeRTOS task. It does nothing
unless provisioned. Wi-Fi must already be provisioned in the ESP32 Wi-Fi
configuration. NVS namespace `crab-inventory` requires:

- `kiosk`: base URL of the Kiosk.
- `device_code`: approved controller identity.
- `device_key`: existing dedicated device credential issued by BE.
- `ca`: root CA PEM for HTTPS. Production must use TLS on both hops.
- `dev_http`: optional boolean, false by default; ONLY for a disposable
  isolated development environment, never production.
- `snapshot`: owned by the reporter, stores revision and descriptor JSON.
  Do not erase during ordinary firmware updates.

The existing controller onboarding must supply these values. No credentials
are committed. This firmware repo currently has no network onboarding
implementation. The manifest is sent to Kiosk via PUT
/api/controllers/{deviceCode}/ports/manifest; Kiosk forwards X-Device-Key
to BE. Retry every 10s on failure, refresh every 60s on success. A new revision
is created automatically if the descriptor list changes. A 409 after erasing
NVS requires recovery/reprovision; do not reset the server revision blindly.

GPIO SSR modules have no digital identity: declaring another GPIO or using
a supported smart module driver is necessary. A descriptor-based bus
discovery driver is a future extension, not implemented by this change.
Never scan GPIO by energizing unknown outputs. Do not treat declared presence
as confirmed load attachment. Network registration, secure local credential
provisioning, hardware-specific RAS control, and on-device tests are required
before deployment.
