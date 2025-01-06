#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <iostream>
#include <fstream>
#include <filesystem>

namespace fs = std::filesystem;

void saveFrameWithoutBoxes(const cv::Mat& originalFrame, int frameNumber, const std::string& outputDir) {
    // Salva o frame original (sem bounding boxes)
    std::string filename = outputDir + "/frame_" + std::to_string(frameNumber) + ".jpg";
    cv::imwrite(filename, originalFrame);
    std::cout << "Salvando frame sem bounding boxes: " << filename << std::endl;
}

int main() {
    // Configurações de salvamento
    const int frameSkip = 50;           // Salvar frames a cada N frames
    const double maxDirectorySize = 500.0; // Limite de tamanho em MB (exemplo: 500MB)
    std::string outputDir = "output/";

    // Cria o diretório de saída, se não existir
    if (!fs::exists(outputDir)) {
        fs::create_directory(outputDir);
    }

    // Caminhos para os arquivos YOLO
    std::string configPath = "./settings/yolov4.cfg";
    std::string weightsPath = "./settings/yolov4.weights";
    std::string namesPath = "./settings/coco.names";

    // Carrega as classes
    std::vector<std::string> classNames;
    std::ifstream ifs(namesPath.c_str());
    std::string line;
    while (std::getline(ifs, line)) {
        classNames.push_back(line);
    }

    // Carrega a rede YOLOv4
    cv::dnn::Net net = cv::dnn::readNetFromDarknet(configPath, weightsPath);
    net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
    net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);

    // Caminho para o vídeo
    std::string videoPath = "./CAMERA/CAM_1/output.mp4";
    cv::VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        std::cerr << "Erro ao abrir o vídeo!" << std::endl;
        return -1;
    }

    int frameNumber = 0;
    cv::Mat frame;

    // Loop principal para processar o vídeo
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        frameNumber++;

        // Aplica o skip de frames
        if (frameNumber % frameSkip != 0) {
            continue; // Ignora frames que não atendem ao critério
        }

        // Salva o frame original antes de desenhar qualquer coisa
        saveFrameWithoutBoxes(frame, frameNumber, outputDir);

        // Copia o frame para exibir as detecções
        cv::Mat displayFrame = frame.clone();

        // Pré-processamento para o YOLO
        cv::Mat blob;
        cv::dnn::blobFromImage(displayFrame, blob, 1 / 255.0, cv::Size(416, 416), cv::Scalar(), true, false);
        net.setInput(blob);

        // Faz a detecção
        std::vector<cv::Mat> outputs;
        net.forward(outputs, net.getUnconnectedOutLayersNames());

        std::vector<int> classIds;
        std::vector<float> confidences;
        std::vector<cv::Rect> boxes;

        for (auto& output : outputs) {
            for (int i = 0; i < output.rows; i++) {
                float* data = (float*)output.ptr(i);
                float confidence = data[4];  // Confiança da detecção
                if (confidence > 0.5) {
                    int centerX = (int)(data[0] * frame.cols);
                    int centerY = (int)(data[1] * frame.rows);
                    int width = (int)(data[2] * frame.cols);
                    int height = (int)(data[3] * frame.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    cv::Rect box(left, top, width, height);
                    boxes.push_back(box);
                    confidences.push_back(confidence);

                    // Identifica a classe
                    int classId = std::max_element(data + 5, data + 5 + classNames.size()) - (data + 5);
                    classIds.push_back(classId);
                }
            }
        }

        // Aplica supressão não máxima para evitar caixas sobrepostas
        std::vector<int> indices;
        cv::dnn::NMSBoxes(boxes, confidences, 0.5, 0.4, indices);

        // for (int idx : indices) {
        //     cv::Rect box = boxes[idx];
        //     int classId = classIds[idx];

        //     // Desenha a caixa delimitadora e o nome da classe no frame para exibição
        //     cv::rectangle(displayFrame, box, cv::Scalar(0, 255, 0), 2);
        //     std::string label = classNames[classId] + ": " + cv::format("%.2f", confidences[idx]);
        //     cv::putText(displayFrame, label, cv::Point(box.x, box.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
        // }

        // // Exibe o frame processado com as detecções
        // cv::imshow("Detecções", displayFrame);
        // if (cv::waitKey(1) >= 0) break;
    }

    cap.release();
    // cv::destroyAllWindows();

    return 0;
}
