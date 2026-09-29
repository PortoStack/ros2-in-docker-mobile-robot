use tauri::{AppHandle, Emitter};

#[tauri::command]
pub fn navigate_to(app: AppHandle, path: String) -> Result<(), String> {
    app.emit("navigate-to", path)
        .map_err(|e| e.to_string())
}
