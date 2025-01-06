# EOG-Based Eye Movement Analysis for Health Monitoring and Assistive Communication

## Overview
This project aims to develop an EOG-based system for analyzing eye movements in two major applications:
1. **Health Monitoring**: Diagnosing and monitoring conditions such as neurological and ocular disorders through eye movement analysis.
2. **Assistive Communication**: Enabling paralyzed individuals to communicate through a specially designed user interface controlled by their eye movements.

## Objectives
- Develop a real-time system to process and analyze EOG signals.
- Use machine learning models to detect and classify eye movements for health monitoring.
- Build an assistive communication interface for individuals with paralysis using eye movements as control input.

## Methodology
1. **Hardware Setup**: 
    - EOG electrodes placed around the eyes for movement detection.
    - Data collection using an Arduino/STM32 platform and BioAmp EXG Pill.
    
2. **Signal Processing**:
    - Preprocessing for noise removal (using filters) and normalization.
    - Feature extraction from EOG signals to detect movement in the horizontal and vertical directions.
    
3. **Health Monitoring**:
    - Machine learning models to monitor specific eye movements associated with health conditions.
    
4. **Assistive Communication**:
    - Real-time interpretation of eye movements to control a user interface for communication.

## Key Features
- **EOG Signal Analysis**: Accurately identify and classify eye movements.
- **Health Monitoring**: Use eye movement data to diagnose and monitor conditions like Nystagmus, Parkinson's disease, and more.
- **Assistive Communication Interface**: Enable individuals with paralysis to interact with devices using eye movements.

## Expected Outcome
- A fully functioning system that allows:
  - **Accurate EOG signal analysis**.
  - **Health condition monitoring** based on eye movement patterns.
  - **Communication through a user-friendly interface** for people with limited mobility.

## Hardware Requirements
- ESP32 NoseMCU - with WiFi connectivity
- BioAmp EXG Pill
- Gel electrodes

## Software Requirements
- Python
- Arduino IDE
