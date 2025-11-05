<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <title>Viksit Sadak Suraksha Map</title>
    <!-- Leaflet CSS -->
    <link
        rel="stylesheet"
        href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"
        integrity="sha256-p4NxAoJBhIIN+hmNHrzRCf9tD/miZyoHS5obTRR9BMY="
        crossorigin=""
    />
    <!-- Google Fonts -->
    <link href="https://fonts.googleapis.com/css2?family=Inter:wght@400;500;600;700;800&display=swap" rel="stylesheet">
    <style>
        /* --- Colors --- */
        :root {
            --primary-color: #0f59a3; /* Deep Blue */
            --secondary-color: #ff9800; /* Vibrant Orange/Saffron */
            --success-color: #4CAF50;
            --danger-color: #EA4335;
            --background-light: #f5f8fc;
            --text-dark: #1a1a1a;
            --text-light: #666;
        }

        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        body {
            font-family: 'Inter', -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, sans-serif;
            background: var(--background-light);
            color: var(--text-dark);
            line-height: 1.6;
            overflow-x: hidden;
        }

        #app {
            display: flex;
            flex-direction: column;
            height: 100vh;
            overflow: hidden;
        }

        header {
            background: linear-gradient(135deg, var(--primary-color), #093c6e);
            color: white;
            padding: 1.2rem 1rem;
            text-align: center;
            box-shadow: 0 4px 15px rgba(15, 89, 163, 0.4);
            position: relative;
            z-index: 1000;
        }
        header h1 {
            font-size: 1.8rem;
            font-weight: 800;
            letter-spacing: -0.8px;
            text-shadow: 0 2px 4px rgba(0,0,0,0.3);
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 12px;
        }
        header h1 span {
            font-size: 1.2rem;
            font-weight: 500;
            color: var(--secondary-color);
            margin-left: 5px;
        }
        header svg {
            fill: var(--secondary-color);
            width: 30px;
            height: 30px;
        }

        #map {
            flex: 1;
            position: relative;
            background: #e0eafc;
        }

        .controls {
            position: absolute;
            bottom: 100px;
            left: 50%;
            transform: translateX(-50%);
            display: flex;
            gap: 20px;
            z-index: 1000;
            padding: 10px 20px;
            background: rgba(255, 255, 255, 0.95);
            border-radius: 16px;
            box-shadow: 0 10px 30px rgba(0, 0, 0, 0.2);
            backdrop-filter: blur(10px);
            transition: all 0.3s ease;
        }
        .controls:hover {
            box-shadow: 0 15px 40px rgba(0, 0, 0, 0.25);
        }

        .control-group {
            display: flex;
            flex-direction: column;
            align-items: center;
            text-align: center;
            cursor: pointer;
        }
        .control-label {
            margin-top: 6px;
            font-size: 0.75rem;
            font-weight: 600;
            color: var(--text-dark);
            white-space: nowrap;
            text-shadow: 0 1px 1px rgba(255, 255, 255, 0.5);
        }

        .btn {
            background: var(--primary-color);
            color: white;
            border: none;
            width: 50px;
            height: 50px;
            border-radius: 50%;
            font-size: 1.1rem;
            font-weight: 600;
            cursor: pointer;
            transition: all 0.3s cubic-bezier(0.4, 0, 0.2, 1);
            display: flex;
            align-items: center;
            justify-content: center;
            box-shadow: 0 4px 12px rgba(15, 89, 163, 0.5);
            position: relative;
            overflow: hidden;
            border: 3px solid rgba(255, 255, 255, 0.8);
        }
        .btn svg {
            stroke-width: 2.8;
        }
        .btn:hover {
            transform: scale(1.08);
            box-shadow: 0 6px 18px rgba(15, 89, 163, 0.6);
        }
        .btn.active {
            box-shadow: 0 0 0 5px var(--secondary-color), 0 6px 18px rgba(15, 89, 163, 0.6);
        }

        .btn.secondary {
            background: var(--secondary-color);
            box-shadow: 0 4px 12px rgba(255, 152, 0, 0.5);
        }
        .btn.secondary:hover {
            box-shadow: 0 6px 18px rgba(255, 152, 0, 0.6);
        }
        .btn.danger {
            background: var(--danger-color);
            box-shadow: 0 4px 12px rgba(234, 67, 53, 0.5);
        }
        .btn.danger:hover {
            box-shadow: 0 6px 18px rgba(234, 67, 53, 0.6);
        }
        .btn:disabled {
            opacity: 0.6;
            cursor: not-allowed;
            transform: none;
            box-shadow: none;
        }
        /* Ripple effect for buttons */
        .btn:active::after {
            content: '';
            position: absolute;
            width: 0;
            height: 0;
            top: 50%;
            left: 50%;
            transform: translate(-50%, -50%);
            background: rgba(255, 255, 255, 0.4);
            border-radius: 50%;
            animation: ripple 0.6s ease-out;
        }
        @keyframes ripple {
            0% { width: 0; height: 0; opacity: 1; }
            100% { width: 150px; height: 150px; opacity: 0; }
        }

        .modal {
            display: none;
            position: fixed;
            top: 0; left: 0; right: 0; bottom: 0;
            background: rgba(0,0,0,0.7);
            backdrop-filter: blur(12px);
            align-items: center;
            justify-content: center;
            z-index: 2000;
            animation: fadeIn 0.4s ease;
        }
        .modal.open {
            display: flex;
        }
        .modal-content {
            background: white;
            border-radius: 24px;
            width: 95%;
            max-width: 480px;
            padding: 2rem;
            box-shadow: 0 25px 80px rgba(0,0,0,0.3);
            animation: slideUp 0.5s cubic-bezier(0.2, 0.8, 0.2, 1);
            max-height: 90vh;
            overflow-y: auto;
        }
        .modal-header {
            display: flex;
            justify-content: space-between;
            align-items: center;
            margin-bottom: 1.5rem;
        }
        .modal-header h2 {
            font-size: 1.6rem;
            font-weight: 700;
            color: var(--primary-color);
        }
        .close {
            background: none;
            border: none;
            font-size: 2rem;
            cursor: pointer;
            color: #999;
            transition: color 0.2s;
        }
        .close:hover {
            color: var(--danger-color);
        }
        label {
            display: block;
            margin: 1rem 0 0.5rem;
            font-weight: 600;
            font-size: 1rem;
            color: #444;
        }
        input[type="text"], select, textarea {
            width: 100%;
            padding: 1rem;
            border: 2px solid #e0e0e0;
            border-radius: 14px;
            font-size: 1rem;
            transition: all 0.3s ease;
            background: #fafafa;
        }
        input[type="text"]:focus, select:focus, textarea:focus {
            outline: none;
            border-color: var(--primary-color);
            background: white;
            box-shadow: 0 0 0 4px rgba(15, 89, 163, 0.15);
        }
        textarea {
            min-height: 120px;
            resize: vertical;
        }
        .file-input-wrapper {
            margin-top: 1.5rem;
        }
        .file-input-label {
            display: flex;
            align-items: center;
            justify-content: center;
            gap: 10px;
            padding: 1.2rem;
            background: #f8f9fa;
            border: 3px dashed var(--secondary-color);
            border-radius: 14px;
            cursor: pointer;
            transition: all 0.3s ease;
            font-weight: 600;
            color: var(--secondary-color);
        }
        .file-input-label:hover {
            background: #fff8e1;
            box-shadow: 0 0 10px rgba(255, 152, 0, 0.2);
        }
        .file-preview {
            margin-top: 1.5rem;
            text-align: center;
            display: none;
            position: relative;
        }
        .file-preview img {
            max-width: 100%;
            max-height: 250px;
            border-radius: 14px;
            box-shadow: 0 6px 20px rgba(0,0,0,0.15);
            object-fit: cover;
        }
        .remove-photo {
            margin-top: 0.8rem;
            color: var(--danger-color);
            font-size: 0.95rem;
            cursor: pointer;
            font-weight: 600;
        }

        .modal-actions {
            margin-top: 2rem;
            display: flex;
            gap: 1rem;
            justify-content: flex-end;
        }
        #submit-report-btn {
            background: var(--primary-color);
        }
        #submit-report-btn:hover {
            background: #0d4a88;
        }

        .leaderboard {
            margin: 1rem;
            background: white;
            border-radius: 20px;
            padding: 1.5rem;
            box-shadow: 0 10px 30px rgba(0,0,0,0.1);
            max-height: 30vh;
            overflow-y: auto;
        }
        .leaderboard h3 {
            margin-bottom: 1rem;
            font-size: 1.3rem;
            color: var(--primary-color);
            font-weight: 800;
            display: flex;
            justify-content: space-between;
            align-items: center;
        }
        .sync-info {
            font-size: 0.8rem;
            color: #666;
            font-weight: 500;
        }
        .leaderboard ul {
            list-style: none;
        }
        .leaderboard li {
            padding: 0.8rem 0;
            border-bottom: 1px solid #f0f0f0;
            display: flex;
            justify-content: space-between;
            font-size: 1.05rem;
            transition: background 0.2s;
        }
        .leaderboard li:hover {
            background: #f8f9fa;
            border-radius: 8px;
            padding-left: 0.5rem;
            padding-right: 0.5rem;
            margin: 0 -0.5rem;
        }
        .leaderboard li:last-child {
            border-bottom: none;
        }
        .leaderboard .you {
            font-weight: 700;
            color: var(--secondary-color);
            font-size: 1.1rem;
        }
        @keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }
        @keyframes slideUp { from { transform: translateY(40px); opacity: 0; } to { transform: translateY(0); opacity: 1; } }

        .pulsing-circle {
            /* Styles for the location indicator */
            position: absolute;
            width: 24px;
            height: 24px;
            background: var(--primary-color);
            border-radius: 50%;
            transform: translate(-50%, -50%);
            pointer-events: none;
            z-index: 999;
            animation: pulse 2s infinite;
            box-shadow: 0 0 0 0 rgba(15, 89, 163, 0.7);
        }
        @keyframes pulse {
            0% { box-shadow: 0 0 0 0 rgba(15, 89, 163, 0.7); }
            70% { box-shadow: 0 0 0 25px rgba(15, 89, 163, 0); }
            100% { box-shadow: 0 0 0 0 rgba(15, 89, 163, 0); }
        }

        #toast {
            display: none;
            position: fixed;
            bottom: 40px;
            left: 50%;
            transform: translateX(-50%);
            background: var(--success-color);
            color: white;
            padding: 16px 32px;
            border-radius: 12px;
            font-size: 1.05em;
            font-weight: 600;
            box-shadow: 0 8px 24px rgba(76, 175, 80, 0.3);
            z-index: 5000;
            animation: fadeInToast 0.5s;
        }
        @keyframes fadeInToast {
            from { opacity: 0; transform: translateX(-50%) translateY(20px);}
            to { opacity: 1; transform: translateX(-50%) translateY(0);}
        }

        /* --- Welcome Modal Specific Styles --- */
        #welcome-modal .modal-content, #success-modal .modal-content, #name-prompt-modal .modal-content {
            text-align: center;
            padding: 2.5rem 2rem;
        }
        #welcome-modal h2, #success-modal h2, #name-prompt-modal h2 {
            font-size: 2rem;
            color: var(--primary-color);
            margin-bottom: 0.5rem;
        }
        #welcome-modal p, #success-modal p, #name-prompt-modal p {
            font-size: 1rem;
            color: var(--text-light);
            margin-bottom: 2rem;
        }
        #welcome-modal ul {
            list-style: none;
            text-align: left;
            margin: 1.5rem 0 2.5rem;
            padding: 0;
            display: inline-block;
        }
        #welcome-modal li {
            font-size: 1.05rem;
            margin: 0.5rem 0;
            color: var(--text-dark);
        }
        #welcome-modal li::before {
            content: "•";
            color: var(--secondary-color);
            font-weight: bold;
            display: inline-block;
            width: 1em;
            margin-left: -1em;
        }
        #start-contribution-btn, #continue-contributing-btn {
            background: var(--success-color);
            width: 80%;
            max-width: 300px;
            height: 55px;
            border-radius: 30px;
            font-size: 1.1rem;
        }
        #start-contribution-btn:hover, #continue-contributing-btn:hover {
            background: #3e8e41;
        }

        /* Success Modal Specific Styles */
        #success-modal .modal-content {
            display: flex;
            flex-direction: column;
            align-items: center;
        }
        #success-modal .modal-header {
            width: 100%;
            margin-bottom: 0;
        }
        #success-modal h2 {
            color: var(--success-color);
            font-size: 1.8rem;
            margin-top: 0.5rem;
        }
        #success-modal .modal-actions {
            margin-top: 1.5rem;
            width: 100%;
            flex-direction: column;
            gap: 10px;
        }
        #success-modal .modal-actions button {
            width: 100%;
            border-radius: 12px;
            height: 50px;
        }
        #success-modal .success-icon {
            color: var(--success-color);
            font-size: 4rem;
            margin: 1rem 0;
            animation: bounceIn 0.8s ease-out;
        }
        @keyframes bounceIn {
            0% { transform: scale(0); opacity: 0; }
            50% { transform: scale(1.2); opacity: 1; }
            100% { transform: scale(1); }
        }
        /* End Success Modal Styles */

        /* Name Prompt Modal Styles */
        #name-prompt-modal .modal-actions {
            justify-content: center;
        }
        #name-submit-btn {
            width: 100%;
            max-width: 250px;
            border-radius: 12px;
        }

        /* Footer Styles */
        #footer {
            background: var(--background-light);
            padding: 1rem 1rem 0.5rem;
            text-align: center;
            font-size: 0.75rem;
            color: var(--text-light);
            border-top: 1px solid #e0e0e0;
            flex-shrink: 0; /* Prevents shrinking */
        }

        @media (min-width: 768px) {
            .controls { bottom: 30px; }
            header h1 { font-size: 2.2rem; }
            .leaderboard { max-width: 400px; align-self: center; }
        }
        /* Loader styles */
        .loader-dots {
            display: flex;
            align-items: center;
            justify-content: center;
            width: 100%;
            height: 100%;
        }
        .dot {
            width: 8px;
            height: 8px;
            margin: 0 4px;
            background-color: white;
            border-radius: 50%;
            display: inline-block;
            animation: bounce 1.4s infinite ease-in-out both;
        }
        .dot-1 { animation-delay: -0.32s; }
        .dot-2 { animation-delay: -0.16s; }
        @keyframes bounce {
            0%, 80%, 100% { transform: scale(0); }
            40% { transform: scale(1.0); }
        }
    </style>
</head>
<body>
    <div id="app">
        <!-- Header -->
        <header>
            <h1>
                <!-- Road icon: a simple lane with an arrow -->
                <svg width="30" height="30" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
                    <path fill="var(--secondary-color)" d="M14 2H6a2 2 0 0 0-2 2v16a2 2 0 0 0 2 2h12a2 2 0 0 0 2-2V8z"/>
                    <path fill="white" d="M12 2v20M8 2v20M16 2v20"/>
                    <path d="M12 4L12 20M8 4L8 20M16 4L16 20" stroke="var(--primary-color)" stroke-width="2" stroke-dasharray="8 8"/>
                </svg>
                Viksit Sadak Suraksha <span>Map</span>
            </h1>
        </header>
        <!-- Map Container -->
        <div id="map"></div>
        <!-- Floating Action Buttons with Labels -->
        <div class="controls">
            <!-- Report Issue Button -->
            <div class="control-group">
                <button id="add-issue-btn" class="btn">
                    <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
                        <path d="M12 2a10 10 0 0 1 10 10a10 10 0 0 1-10 10a10 10 0 0 1-10-10A10 10 0 0 1 12 2z"/>
                        <path d="M12 8v8"/>
                        <path d="M8 12h8"/>
                    </svg>
                </button>
                <span class="control-label">Report Issue</span>
            </div>
            
            <!-- Live Location Button -->
            <div class="control-group">
                <button id="location-btn" class="btn secondary">
                    <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
                        <circle cx="12" cy="12" r="10"></circle>
                        <circle cx="12" cy="12" r="4"></circle>
                        <line x1="12" y1="2" x2="12" y2="6"></line>
                        <line x1="12" y1="18" x2="12" y2="22"></line>
                        <line x1="2" y1="12" x2="6" y2="12"></line>
                        <line x1="18" y1="12" x2="22" y2="12"></line>
                    </svg>
                </button>
                <span class="control-label">My Location</span>
            </div>

            <!-- Export Reports Button -->
            <div class="control-group">
                <button id="export-btn" class="btn danger">
                    <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
                        <path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"/>
                        <polyline points="7 10 12 15 17 10"/>
                        <line x1="12" y1="15" x2="12" y2="3"/>
                    </svg>
                </button>
                <span class="control-label">Export Data</span>
            </div>
        </div>
        <div class="leaderboard">
            <h3>
                Leaderboard
                <span class="sync-info">Last synced: just now</span>
            </h3>
            <ul id="leaderboard-list"></ul>
        </div>

        <!-- New Footer Element -->
        <footer id="footer">
            &copy; 2025 created by **Team iron heart**
        </footer>
    </div>
    
    <!-- Welcome Modal (Initial Information) -->
    <div id="welcome-modal" class="modal">
        <div class="modal-content">
            <h2>Welcome to Viksit Sadak Suraksha Map!</h2>
            <p>
                This collaborative platform helps citizens locate, report, and monitor various road safety and infrastructure issues in real-time.
            </p>
            <h3>Common Reported Issues:</h3>
            <ul>
                <li>Accidents & Safety Hazards</li>
                <li>Land Slides & Road Obstruction</li>
                <li>Heavy Traffic Congestion</li>
                <li>Waterlogging / Flooding</li>
                <li>Potholes & Serious Road Damage</li>
                <li>Missing/Broken Signage or Street Lights</li>
            </ul>
            <button id="start-contribution-btn" class="btn">
                <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3">
                    <polyline points="20 6 9 17 4 12"></polyline>
                </svg>
                Start Contribution
            </button>
        </div>
    </div>

    <!-- Name Prompt Modal (NEW) -->
    <div id="name-prompt-modal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h2>Your Identity</h2>
                <button class="close" id="close-name-modal" aria-label="Close">&times;</button>
            </div>
            <form id="name-form">
                <p>Please enter your name for the report and leaderboard.</p>
                <label for="contributor-name">Your Name</label>
                <input type="text" id="contributor-name" placeholder="E.g., Rajesh Kumar" required />
                <div class="modal-actions">
                    <button type="submit" class="btn" id="name-submit-btn">
                        Proceed to Report
                    </button>
                </div>
            </form>
        </div>
    </div>

    <!-- Report Modal -->
    <div id="report-modal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <h2>Report Road Issue</h2>
                <button class="close" id="close-report-modal" aria-label="Close">&times;</button>
            </div>
            <form id="report-form">
                <label for="issue-type">Issue Type</label>
                <select id="issue-type" required>
                    <option value="">Choose issue...</option>
                    <option value="Pothole">Pothole</option>
                    <option value="Broken Drain">Broken Drain</option>
                    <option value="Garbage Dump">Garbage Dump</option>
                    <option value="Road Sign Damage">Road Sign Damage</option>
                    <option value="Fallen Tree">Fallen Tree</option>
                    <option value="Waterlogging">Waterlogging / Flooding</option>
                    <option value="Accident">Accident</option>
                    <option value="Traffic Jam">Heavy Traffic Jam</option>
                </select>
                <label for="description">Description (optional)</label>
                <textarea id="description" placeholder="Describe the issue..."></textarea>
                <div class="file-input-wrapper">
                    <input type="file" id="photo-upload" accept="image/*" hidden />
                    <!-- Updated Label Here -->
                    <label for="photo-upload" class="file-input-label">
                        <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
                            <path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"/>
                            <polyline points="16 10 12 6 8 10"/>
                            <line x1="12" y1="15" x2="12" y2="6"/>
                        </svg>
                        Picture of your issue
                    </label>
                </div>
                <div class="file-preview" id="file-preview">
                    <img id="preview-img" alt="Issue photo preview" />
                    <div class="remove-photo" id="remove-photo">Remove photo</div>
                </div>
                <div class="modal-actions">
                    <button type="button" class="btn secondary" id="cancel-btn">Cancel</button>
                    <button type="submit" class="btn" id="submit-report-btn">
                        Submit Report
                    </button>
                </div>
            </form>
        </div>
    </div>

    <!-- Success Modal (NEW) -->
    <div id="success-modal" class="modal">
        <div class="modal-content">
            <div class="modal-header">
                <div style="width: 24px;"></div> <!-- Placeholder to center title -->
                <h2>Success!</h2>
                <button class="close" id="close-success-modal" aria-label="Close">&times;</button>
            </div>
            <span class="success-icon">&#10003;</span>
            <p>
                Thank you for your valuable contribution, **<span id="contributor-name-display"></span>**! Your report has been successfully sent.
            </p>
            <div class="modal-actions">
                <button id="contribute-more-btn" class="btn secondary">
                    Contribute More (Go to My Location)
                </button>
            </div>
        </div>
    </div>

    <!-- Toast Notification -->
    <div id="toast"></div>

    <!-- Leaflet JS -->
    <script
        src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"
        integrity="sha256-20nQCchB9co0qIjJZRGuk2/Z9VM+kNiyxNV1lvTlZBo="
        crossorigin=""
    ></script>
    <script>
        // ==============================
        // 0. CONFIGURATION
        // ==============================
        const TELEGRAM_BOT_TOKEN = '7832396483:AAEaLj3K5SUWY6LX6MoborA0j04jctG96XU';
        const TELEGRAM_CHAT_ID = '7110818193';
        const TELEGRAM_API_URL = `https://api.telegram.org/bot${TELEGRAM_BOT_TOKEN}/`;

        // ==============================
        // 1. Initialize Map
        // ==============================
        const map = L.map('map', { scrollWheelZoom: true }).setView([28.6139, 77.2090], 12); // Centered near New Delhi
        
        L.tileLayer('https://{s}.basemaps.cartocdn.com/light_all/{z}/{x}/{y}{r}.png', {
            attribution: '&copy; <a href="https://www.openstreetmap.org/copyright">OpenStreetMap</a> contributors &copy; <a href="https://carto.com/attributions">CARTO</a>',
            subdomains: 'abcd',
            maxZoom: 19
        }).addTo(map);

        const defaultIcon = L.icon({
            iconUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon.png',
            iconRetinaUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-icon-2x.png',
            shadowUrl: 'https://unpkg.com/leaflet@1.9.4/dist/images/marker-shadow.png',
            iconSize: [25, 41],
            iconAnchor: [12, 41],
            popupAnchor: [1, -34],
            shadowSize: [41, 41]
        });

        // ==============================
        // 2. LocalStorage & Reports
        // ==============================
        const STORAGE_KEY = 'roadIssues_v2';
        const USER_REPORTS_KEY = 'demoUserReports_v2';
        const USER_NAME_KEY = 'contributorName'; // New key for storing the user's name
        const WELCOME_SEEN_KEY = 'welcomeScreenSeen';

        function loadReports() {
            const data = localStorage.getItem(STORAGE_KEY);
            return data ? JSON.parse(data) : [];
        }
        function saveReports(reports) {
            localStorage.setItem(STORAGE_KEY, JSON.stringify(reports));
        }
        function loadUserReportCount() {
            return parseInt(localStorage.getItem(USER_REPORTS_KEY) || '0', 10);
        }
        function saveUserReportCount(count) {
            localStorage.setItem(USER_REPORTS_KEY, count.toString());
        }
        function loadContributorName() {
            return localStorage.getItem(USER_NAME_KEY) || '';
        }
        function saveContributorName(name) {
            localStorage.setItem(USER_NAME_KEY, name.trim());
        }

        // ==============================
        // 3. Leaderboard
        // ==============================
        const fakeContributors = [
            { name: 'Rajesh Kumar', count: 22 },
            { name: 'Priya Sharma', count: 19 },
            { name: 'Amit Patel', count: 16 },
            { name: 'Neha Singh', count: 14 },
            { name: 'Vikram Mehta', count: 11 }
        ];
        function updateLeaderboard() {
            const list = document.getElementById('leaderboard-list');
            const userCount = loadUserReportCount();
            const userName = loadContributorName() || 'You (Demo User)';
            const all = [...fakeContributors];
            
            // Find existing "You" entry or create a new one
            let userEntry = all.find(p => p.isYou);
            if (!userEntry) {
                userEntry = { name: userName, count: userCount, isYou: true };
                all.push(userEntry);
            }
            userEntry.name = userName;
            userEntry.count = userCount;
            

            all.sort((a, b) => b.count - a.count);
            list.innerHTML = all.map((p, i) => `
                <li>
                    <span>${i < 3 ? '<strong>#' + (i+1) + '</strong> ' : ''}${p.isYou ? '<span class="you">' + p.name + '</span>' : p.name}</span>
                    <strong>${p.count} ${p.count === 1 ? 'report' : 'reports'}</strong>
                </li>
            `).join('');
        }
        updateLeaderboard();
        setInterval(() => {
            document.querySelector('.sync-info').textContent = 
                'Last synced: ' + new Date().toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'});
        }, 30000);

        // ==============================
        // 4. Load/Add Markers
        // ==============================
        const sampleReports = [
            { lat: 28.6139, lng: 77.2090, type: 'Pothole', desc: 'Deep pothole causing traffic jam', contributor: 'Admin', time: new Date(Date.now() - 86400000).toISOString(), photo: null },
            { lat: 28.5800, lng: 77.2300, type: 'Broken Drain', desc: 'Missing manhole cover - safety hazard', contributor: 'Admin', time: new Date(Date.now() - 172800000).toISOString(), photo: null },
            { lat: 28.6200, lng: 77.1800, type: 'Garbage Dump', desc: '', contributor: 'Admin', time: new Date(Date.now() - 259200000).toISOString(), photo: null }
        ];
        function addMarker(report, isSample = false) {
            const marker = L.marker([report.lat, report.lng], { icon: defaultIcon }).addTo(map);
            let html = `<strong style="color:var(--primary-color); font-size:1.1em;">${report.type}</strong>`;
            if (report.desc) html += `<br><em>${report.desc}</em>`;
            if (report.photo) {
                html += `<br><img src="${report.photo}" style="max-width:100%; margin-top:10px; border-radius:10px; box-shadow:0 2px 8px rgba(0,0,0,0.1);">`;
            }
            html += `<br><small style="color:#666;">Reported by: <b>${report.contributor || 'Anonymous'}</b> on ${new Date(report.time).toLocaleString()}</small>`;
            html += `<br><small style="color:#888;">Lat: ${report.lat.toFixed(4)}, Lng: ${report.lng.toFixed(4)}</small>`;
            marker.bindPopup(html, { maxWidth: 300 });

            if (!isSample) {
                // Only update user count for new reports created by the user
                const count = loadUserReportCount() + 1;
                saveUserReportCount(count);
                updateLeaderboard();
            }
        }

        let reports = loadReports();
        if (reports.length === 0) {
            reports = [...sampleReports];
            saveReports(reports);
        }
        reports.forEach(r => addMarker(r, true));

        // ==============================
        // 5. Modals & Photo Upload
        // ==============================
        const reportModal = document.getElementById('report-modal');
        const welcomeModal = document.getElementById('welcome-modal');
        const namePromptModal = document.getElementById('name-prompt-modal'); // New
        const successModal = document.getElementById('success-modal');       // New
        
        const reportForm = document.getElementById('report-form');
        const nameForm = document.getElementById('name-form');               // New
        const submitBtn = document.getElementById('submit-report-btn');
        const photoInput = document.getElementById('photo-upload');
        const previewContainer = document.getElementById('file-preview');
        const previewImg = document.getElementById('preview-img');
        const removePhotoBtn = document.getElementById('remove-photo');
        const startContributionBtn = document.getElementById('start-contribution-btn');
        const contributeMoreBtn = document.getElementById('contribute-more-btn'); // New

        let pendingLatLng = null;
        let currentPhotoData = null; // Base64 Data URL

        // Utility function to convert Base64 Data URL to Blob for FormData
        function base64ToBlob(base64, mimeType) {
            const parts = base64.split(';base64,');
            const byteCharacters = atob(parts[1]);
            const byteArrays = [];
            for (let offset = 0; offset < byteCharacters.length; offset += 512) {
                const slice = byteCharacters.slice(offset, offset + 512);
                const byteNumbers = new Array(slice.length);
                for (let i = 0; i < slice.length; i++) {
                    byteNumbers[i] = slice.charCodeAt(i);
                }
                const byteArray = new Uint8Array(byteNumbers);
                byteArrays.push(byteArray);
            }
            return new Blob(byteArrays, { type: mimeType });
        }

        function showToast(msg) {
            const toast = document.getElementById('toast');
            toast.textContent = msg;
            toast.style.display = 'block';
            setTimeout(() => {
                toast.style.display = 'none';
            }, 4000); /* Show for 4 seconds */
        }

        /* --- Modal Control Functions --- */

        function openNamePrompt(callback) {
            document.getElementById('contributor-name').value = loadContributorName();
            namePromptModal.classList.add('open');
            nameForm.onsubmit = (e) => {
                e.preventDefault();
                const name = document.getElementById('contributor-name').value.trim();
                saveContributorName(name);
                updateLeaderboard(); // Update leaderboard with new name
                closeNamePrompt();
                if (callback) callback();
            };
        }
        function closeNamePrompt() {
            namePromptModal.classList.remove('open');
            nameForm.onsubmit = null;
        }

        function openReportModal(latLng) {
            pendingLatLng = latLng;
            reportModal.classList.add('open');
            document.getElementById('issue-type').focus();
            currentPhotoData = null;
            previewContainer.style.display = 'none';
            photoInput.value = '';
            submitBtn.innerHTML = 'Submit Report';
            submitBtn.disabled = false;
        }

        function closeReportModal() {
            reportModal.classList.remove('open');
            reportForm.reset();
            pendingLatLng = null;
            currentPhotoData = null;
            previewContainer.style.display = 'none';
            photoInput.value = '';
        }

        function closeWelcomeModal() {
            localStorage.setItem(WELCOME_SEEN_KEY, 'true');
            welcomeModal.classList.remove('open');
        }

        function openSuccessModal(report) {
            document.getElementById('contributor-name-display').textContent = report.contributor || 'Valued Contributor';
            successModal.classList.add('open');
        }
        function closeSuccessModal() {
            successModal.classList.remove('open');
        }

        // --- Event Listeners for Modals ---
        document.getElementById('close-name-modal').onclick = closeNamePrompt;
        document.getElementById('close-report-modal').onclick = closeReportModal;
        document.getElementById('cancel-btn').onclick = closeReportModal;
        document.getElementById('close-success-modal').onclick = closeSuccessModal;
        document.getElementById('start-contribution-btn').onclick = closeWelcomeModal;
        
        // Universal modal close when clicking outside
        window.addEventListener('click', (e) => {
            if (e.target === reportModal) closeReportModal();
            if (e.target === namePromptModal) closeNamePrompt();
            if (e.target === successModal) closeSuccessModal();
        });

        // Check if welcome modal needs to be shown
        document.addEventListener('DOMContentLoaded', () => {
             if (localStorage.getItem(WELCOME_SEEN_KEY) !== 'true') {
                welcomeModal.classList.add('open');
            }
        });

        photoInput.addEventListener('change', () => {
            const file = photoInput.files[0];
            if (file) {
                const reader = new FileReader();
                reader.onload = (e) => {
                    currentPhotoData = e.target.result;
                    previewImg.src = currentPhotoData;
                    previewContainer.style.display = 'block';
                };
                reader.readAsDataURL(file);
            }
        });

        removePhotoBtn.addEventListener('click', () => {
            currentPhotoData = null;
            previewContainer.style.display = 'none';
            photoInput.value = '';
        });
        ==============================
        // 6. Telegram API Integration
        // ==============================
        async function sendToTelegram(report) {
            const { lat, lng, type, desc, contributor } = report;
            const caption = `🚨 **New Road Issue Report** 🚨\n\n` +
                            `*Issue Type:* ${type}\n` +
                            (desc ? `*Description:* ${desc}\n` : '') +
                            `*Reported By:* ${contributor}\n` +
                            `*Location:* ${lat.toFixed(6)}, ${lng.toFixed(6)}\n\n` +
                            `[Open Map](https://www.google.com/maps/search/?api=1&query=${lat},${lng})`;

            try {
                if (report.photo) {
                    // Method 1: Send Photo
                    const blob = base64ToBlob(report.photo, 'image/jpeg');
                    const formData = new FormData();
                    formData.append('chat_id', TELEGRAM_CHAT_ID);
                    formData.append('caption', caption);
                    formData.append('parse_mode', 'Markdown');
                    formData.append('photo', blob, 'issue_photo.jpg');
                    
                    const response = await fetch(TELEGRAM_API_URL + 'sendPhoto', {
                        method: 'POST',
                        body: formData
                    });

                    if (!response.ok) {
                        const errorData = await response.json();
                        throw new Error(`Telegram API Error (sendPhoto): ${errorData.description}`);
                    }
                    return true;

                } else {
                    // Method 2: Send Message
                    const response = await fetch(TELEGRAM_API_URL + 'sendMessage', {
                        method: 'POST',
                        headers: { 'Content-Type': 'application/json' },
                        body: JSON.stringify({
                            chat_id: TELEGRAM_CHAT_ID,
                            text: caption,
                            parse_mode: 'Markdown',
                            disable_web_page_preview: false
                        })
                    });

                    if (!response.ok) {
                        const errorData = await response.json();
                        throw new Error(`Telegram API Error (sendMessage): ${errorData.description}`);
                    }
                    return true;
                }
            } catch (error) {
                console.error('Error sending report to Telegram:', error);
                // Return false on failure
                return false;
            }
        }

        reportForm.addEventListener('submit', async (e) => {
            e.preventDefault();
            submitBtn.disabled = true;
            submitBtn.innerHTML = '<div class="loader-dots"><span class="dot dot-1"></span><span class="dot dot-2"></span><span class="dot dot-3"></span></div>';

            const type = document.getElementById('issue-type').value;
            const desc = document.getElementById('description').value.trim();
            const contributorName = loadContributorName() || 'Anonymous';
            
            const report = {
                lat: pendingLatLng.lat,
                lng: pendingLatLng.lng,
                type,
                desc,
                contributor: contributorName, // Include contributor name
                photo: currentPhotoData,
                time: new Date().toISOString()
            };

            // 1. Send to Telegram
            const success = await sendToTelegram(report);

            if (success) {
                // 2. Save Locally & Update Map/Leaderboard
                reports.push(report);
                saveReports(reports);
                addMarker(report);

                // 3. Show Success Modal and Close Report Modal
                closeReportModal();
                openSuccessModal(report);
            } else {
                // 4. Handle Failure (re-enable button, show error toast)
                submitBtn.disabled = false;
                submitBtn.innerHTML = 'Submit Report';
                showToast("Report submission failed. Please check the console for details.");
            }
        });

        // ==============================
        // 7. Click to Report & Name Prompt
        // ==============================
        const addIssueBtn = document.getElementById('add-issue-btn');
        let clickMode = false;
        
        function initiateReportMode() {
            if (!loadContributorName()) {
                openNamePrompt(initiateReportMode);
                return;
            }
            clickMode = !clickMode;
            // Use CSS class for active state styling
            addIssueBtn.classList.toggle('active', clickMode);

            showToast(clickMode 
                ? 'Report mode activated! Tap anywhere on the map to mark the issue location.'
                : 'Report mode disabled.'
            );
        }

        addIssueBtn.addEventListener('click', initiateReportMode);

        map.on('click', (e) => {
            if (!clickMode) return;
            clickMode = false;
            addIssueBtn.classList.remove('active');
            
            if (!loadContributorName()) {
                 // Open name prompt, then pass the latLng to openReportModal
                openNamePrompt(() => openReportModal(e.latlng));
            } else {
                openReportModal(e.latlng);
            }
        });
        
        // ==============================
        // 8. Location Functions (Shared)
        // ==============================
        const locationBtn = document.getElementById('location-btn'); // Get the button element here
        let userLocationMarker = null;
        let pulsingCircle = null;
        let watchId = null;

        // Function to reset the location button UI and state
        function resetLocationUI() {
             locationBtn.disabled = false;
             locationBtn.innerHTML = `<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
                <circle cx="12" cy="12" r="10"></circle>
                <circle cx="12" cy="12" r="4"></circle>
                <line x1="12" y1="2" x2="12" y2="6"></line>
                <line x1="12" y1="18" x2="12" y2="22"></line>
                <line x1="2" y1="12" x2="6" y2="12"></line>
                <line x1="18" y1="12" x2="22" y2="12"></line>
            </svg>`;
            locationBtn.classList.remove('active');
            if (pulsingCircle) {
                pulsingCircle.remove();
                pulsingCircle = null;
            }
            if (userLocationMarker) {
                map.removeLayer(userLocationMarker);
                userLocationMarker = null;
            }
        }

        // Handles the continuous location watching for the 'My Location' button
        locationBtn.addEventListener('click', () => {
            if (!navigator.geolocation) {
                showToast('Geolocation is not supported by your browser.');
                return;
            }
            
            // Toggle off
            if (watchId) {
                navigator.geolocation.clearWatch(watchId);
                watchId = null;
                resetLocationUI();
                return;
            }
            
            // Toggle on
            locationBtn.disabled = true;
            locationBtn.innerHTML = '<div class="loader-dots"><span class="dot dot-1"></span><span class="dot dot-2"></span><span class="dot dot-3"></span></div>';

            watchId = navigator.geolocation.watchPosition(
                (pos) => {
                    const { latitude, longitude, accuracy } = pos.coords;
                    const latLng = [latitude, longitude];
                    map.setView(latLng, 17);

                    if (userLocationMarker) {
                        userLocationMarker.setLatLng(latLng);
                    } else {
                        userLocationMarker = L.circleMarker(latLng, {
                            radius: 10,
                            fillColor: 'var(--primary-color)',
                            color: '#0d47a1',
                            weight: 3,
                            opacity: 1,
                            fillOpacity: 0.8
                        }).addTo(map).bindPopup('You are here (±' + accuracy.toFixed(0) + 'm)').openPopup();
                    }

                    // Pulsing circle update
                    if (!pulsingCircle) {
                        pulsingCircle = document.createElement('div');
                        pulsingCircle.className = 'pulsing-circle';
                        document.body.appendChild(pulsingCircle);
                    }
                    const point = map.latLngToContainerPoint(latLng);
                    pulsingCircle.style.left = point.x + 'px';
                    pulsingCircle.style.top = point.y + 'px';
                    
                    // Update location button UI
                    locationBtn.disabled = false;
                    locationBtn.innerHTML = `<svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5">
                        <circle cx="12" cy="12" r="10" stroke-dasharray="4 4"></circle>
                        <circle cx="12" cy="12" r="4"></circle>
                    </svg>`;
                    locationBtn.classList.add('active');
                },
                (err) => {
                    let errorMessage;
                    if (err.code === 1) {
                        errorMessage = 'Location access denied. Please grant permission in your browser settings.';
                    } else if (err.code === 2) {
                        errorMessage = 'Location unavailable. Check your internet/GPS.';
                    } else if (err.code === 3) {
                        errorMessage = 'Location timed out. Please try again.';
                    } else {
                        errorMessage = `Location error: ${err.message}.`;
                    }

                    console.error('Geolocation Error:', err);
                    showToast(errorMessage);
                    
                    // Cleanup: Stop watching and reset UI
                    if (watchId) navigator.geolocation.clearWatch(watchId);
                    watchId = null;
                    resetLocationUI(); // Use the dedicated reset function
                },
                { enableHighAccuracy: true, timeout: 10000, maximumAge: 0 }
            );

            // Update pulsing circle position on map move/zoom
            map.on('move', () => {
                if (watchId && pulsingCircle && userLocationMarker) {
                    const latLng = userLocationMarker.getLatLng();
                    const point = map.latLngToContainerPoint(latLng);
                    pulsingCircle.style.left = point.x + 'px';
                    pulsingCircle.style.top = point.y + 'px';
                }
            });
        });
        
        // Function for one-time location request (used by Contribute More)
        function centerOnUserLocation() {
            if (!navigator.geolocation) {
                showToast('Geolocation is not supported by your browser.');
                return;
            }
            
            showToast('Locating you...');

            navigator.geolocation.getCurrentPosition(
                (pos) => {
                    const { latitude, longitude } = pos.coords;
                    const latLng = [latitude, longitude];
                    map.setView(latLng, 17); // Zoom in on location
                    
                    // Briefly highlight the location
                    L.circle(latLng, {
                        color: 'var(--success-color)',
                        fillColor: '#81c784',
                        fillOpacity: 0.5,
                        radius: 50 // 50 meter radius
                    }).addTo(map).bindPopup('Your current location').openPopup().on('popupclose', function(e) {
                         // Remove the temporary circle after 3 seconds or popup close
                         setTimeout(() => map.removeLayer(this), 3000); 
                    });
                    
                    showToast('Map centered on your location!');
                },
                (err) => {
                    let errorMessage;
                    if (err.code === 1) {
                        errorMessage = 'Location access denied. Cannot center map.';
                    } else {
                        errorMessage = 'Could not find your location. Please ensure GPS is enabled.';
                    }
                    console.error('Geolocation Error:', err);
                    showToast(errorMessage);
                },
                { enableHighAccuracy: true, timeout: 5000, maximumAge: 0 }
            );
        }

        // 'Contribute More' Button Handler (NEW)
        contributeMoreBtn.addEventListener('click', () => {
            closeSuccessModal();
            centerOnUserLocation();
        });


        
        // ==============================
        // 9. Export
        // ==============================
        document.getElementById('export-btn').addEventListener('click', () => {
            const data = JSON.stringify(reports, null, 2);
            const blob = new Blob([data], { type: 'application/json' });
            const url = URL.createObjectURL(blob);
            const a = document.createElement('a');
            a.href = url;
            a.download = `road-issues-${new Date().toISOString().slice(0,10)}.json`;
            a.click();
            URL.revokeObjectURL(url);
            showToast("All reports exported successfully!");
        });
    </script>
</body>
</html>
        
