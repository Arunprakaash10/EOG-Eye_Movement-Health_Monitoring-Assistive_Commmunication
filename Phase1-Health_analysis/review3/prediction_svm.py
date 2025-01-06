import pandas as pd
import joblib

# Load the trained SVM model and label encoder
svm_model = joblib.load('svm_model.pkl')
label_encoder = joblib.load('label_encoder.pkl')

# Load the new dataset (for prediction) - each row is for 10 seconds of data
test_data = pd.read_csv('serial_output.csv')  # Replace with your actual new dataset path

# Accumulate 360 rows (1 hour data) into hourly data
X_new = test_data[['Up Count', 'Down Count', 'Right Count', 'Left Count', 'Blink Count']].groupby(test_data.index // 360).sum()

# Predict on the new dataset (hourly accumulated data)
y_pred_new = svm_model.predict(X_new)

# Decode the predictions back to disease labels
predicted_diseases = label_encoder.inverse_transform(y_pred_new)

# Output the predictions
for i, disease in enumerate(predicted_diseases):
    print(f"Hour {i + 1} predicted disease: {disease}")