# NO2 / NO3 measurement boundary

This board currently performs the NO2 fluid-handling sequence. AIService and ApiService are placeholders; the board has no verified capture/inference or authenticated Sensor ingestion implementation. A completed dosing/drain sequence is not a completed chemical analysis. It must not emit a fake value or mark a Sensor result successful.

For an actual result, use the calibrated camera/analysis pipeline, preserve sample identity and measured time, verify reagent timing and units (mg/L and analyte basis), and report via the approved BE water-analysis workflow. The resulting analytical measurement must be linked to the correct Sensor/area and must not be confused with realtime GPIO/RS485 data. NO3 hardware and analysis are not implemented here. Physical testing, reagent calibration, and validation are required. Relay/pump timing and wiring are unchanged by this change.
