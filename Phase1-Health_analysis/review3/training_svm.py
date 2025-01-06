import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
from sklearn.metrics import classification_report, confusion_matrix, ConfusionMatrixDisplay
from sklearn.preprocessing import LabelEncoder
import joblib

# Load the training dataset
train_data = pd.read_csv('training_dataset.csv')  # Replace with your actual dataset path

# Extract features (Up Count, Down Count, Right Count, Left Count, Blink Count) and target (Condition)
X = train_data[['Up Count', 'Down Count', 'Right Count', 'Left Count', 'Blink Count']]
y = train_data['Condition']

# Encode the labels (diseases) into numeric format
label_encoder = LabelEncoder()
y_encoded = label_encoder.fit_transform(y)

# Split the dataset into training and testing sets
X_train, X_test, y_train, y_test = train_test_split(X, y_encoded, test_size=0.2, random_state=42, stratify=y_encoded)

# Initialize the SVM model
svm_model = SVC(kernel='linear', class_weight='balanced', random_state=42)

# Train the model
svm_model.fit(X_train, y_train)

# Predict on the test set
y_pred = svm_model.predict(X_test)

# Print the classification report
print("Classification Report:")
print(classification_report(y_test, y_pred, target_names=label_encoder.classes_, zero_division=1))

# Generate the confusion matrix
conf_matrix = confusion_matrix(y_test, y_pred)

# Display the confusion matrix
disp = ConfusionMatrixDisplay(confusion_matrix=conf_matrix, display_labels=label_encoder.classes_)
disp.plot(cmap='Blues')
disp.ax_.set_title("Confusion Matrix")
disp.figure_.show()

# Explanation of confusion matrix elements
print("\nConfusion Matrix Explanation:")
print("Each row represents the actual condition (true labels), while each column represents the predicted condition (predicted labels).")
print("For example:")
for i, class_name in enumerate(label_encoder.classes_):
    print(f"- Row {i+1} (Actual: {class_name}):")
    print(f"  - True Positives (Correctly predicted as {class_name}): {conf_matrix[i, i]}")
    print(f"  - False Negatives (Actual {class_name} predicted as other classes): {sum(conf_matrix[i]) - conf_matrix[i, i]}")
    print(f"- Column {i+1} (Predicted as {class_name}):")
    print(f"  - False Positives (Other classes incorrectly predicted as {class_name}): {sum(conf_matrix[:, i]) - conf_matrix[i, i]}\n")

# Save the trained model for future use
joblib.dump(svm_model, 'svm_model.pkl')
joblib.dump(label_encoder, 'label_encoder.pkl')

print("Model and Label Encoder saved successfully.")