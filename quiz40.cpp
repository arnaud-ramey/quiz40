/*!
  \file        camera_opencv_sample.cpp
  \author      Arnaud Ramey <arnaud.a.ramey@gmail.com>
                -- Robotics Lab, University Carlos III of Madrid
  \date        2013/9/30

________________________________________________________________________________

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
________________________________________________________________________________

 */
// C++
#include <algorithm>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// C
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

// https://stackoverflow.com/questions/3418231/replace-part-of-a-string-with-another-string
bool replace(std::string& str, const std::string& from, const std::string& to) {
  size_t start_pos = str.find(from);
  if(start_pos == std::string::npos)
    return false;
  str.replace(start_pos, from.length(), to);
  return true;
}

// https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
std::vector<std::string> files_in_folder(const std::string & folder) {
  std::vector<std::string> ans;
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (folder.c_str())) == NULL)  {
    perror ("");
    return ans;
  }
  /* print all the files and directories within directory */
  while ((ent = readdir (dir)) != NULL)
    ans.push_back(folder + "/" + ent->d_name);
  closedir (dir);
  return ans;
}

int main(int argc, char** argv) {
  srand(time(NULL));
  cv::Size target_size(800, 600);
  static const std::string WINNAME = "quiz40";

  // try to find nquestions questions
  if (argc < 2) {
    printf("Synopsis: %s FOLDER [NQUESTIONS]\n", argv[0]);
    return 0;
  }
  unsigned int nquestions = 40;
  if (argc >= 3)
    nquestions = atoi(argv[2]);
  std::vector<std::string> filenames = files_in_folder(argv[1]);
  std::random_shuffle(filenames.begin(), filenames.end());
  std::vector<cv::Mat3b> questions, answers, errors;
  for (unsigned int i = 1; i <= filenames.size(); ++i) {
    std::string qfilename = filenames[i];
    printf("%i/%i: testing '%s'...\n",
           questions.size(), nquestions, qfilename.c_str());
    if (qfilename.find("q.jp") == std::string::npos) { // not an image
      printf("%i/%i: '%s' is not a question image\n",
             questions.size(), nquestions, qfilename.c_str());
      continue;
    }
    std::string afilename = qfilename;
    replace(afilename, "q.jp", ".jp");
    if (std::find(filenames.begin(), filenames.end(), afilename) == filenames.end()) {
      printf("%i/%i: associated answer '%s' does not exist\n",
             questions.size(), nquestions, qfilename.c_str());
      continue;
    }

    cv::Mat3b question = cv::imread(qfilename, CV_LOAD_IMAGE_COLOR);
    if (question.empty()) {
      printf("%i/%i: could not read question image '%s'!\n",
             questions.size(), nquestions, qfilename.c_str());
      continue;
    }
    cv::Mat3b answer = cv::imread(afilename, CV_LOAD_IMAGE_COLOR);
    if (answer.empty()) {
      printf("%i/%i: could not read answer image '%s'!\n",
             questions.size(), nquestions, afilename.c_str());
      continue;
    }
    double qratio = std::min(1. * target_size.width  / question.cols,
                             1. * target_size.height / question.rows);
    cv::resize(question, question, cv::Size(), qratio, qratio);
    double aratio = std::min(1. * target_size.width  / answer.cols,
                             1. * target_size.height / answer.rows);
    cv::resize(answer, answer, cv::Size(), aratio, aratio);
    printf("%i/%i: adding '%s'\n",
           questions.size(), nquestions, qfilename.c_str());
    questions.push_back(question);
    answers.push_back(answer);
    if (questions.size() == nquestions)
      break;
  } // end for i
  // check we got nquestions questions
  if (questions.size() < nquestions) {
    printf("Could only find %i questions, we wanted %i!\n",
           questions.size(), nquestions);
    return -1;
  }

  // real quiz
  cv::namedWindow(WINNAME);

  unsigned int score = 0;
  for (unsigned int nq = 0; nq < nquestions; ++nq) {
    std::ostringstream score_txt;
    score_txt << score << "/" << nq;
    cv::putText(questions[nq], score_txt.str(), cv::Point(10, 50),
                CV_FONT_HERSHEY_PLAIN, 5, CV_RGB(0, 255, 0), 2);
    cv::putText(answers[nq], score_txt.str(), cv::Point(10, 50),
                CV_FONT_HERSHEY_PLAIN, 5, CV_RGB(0, 255, 0), 2);
    cv::imshow(WINNAME, questions[nq]);
    cv::waitKey(0);
    cv::imshow(WINNAME, answers[nq]);
    char c = cv::waitKey(0);
    if (c == 'y' || c == '1')
      ++score;
    else {
      errors.push_back(questions[nq]);
      errors.push_back(answers[nq]);
    }
  } // end for nq

  printf("Final score:%i/%i\n", score, nquestions);
  if (score == nquestions)
    return 0;
  unsigned int i = 0;
  while (true) {
    cv::imshow(WINNAME, errors[i]);
    char c = cv::waitKey(0);
    if (c == 'q' || c == 27) // quit
      break;
    else if (c == 81) // left
      i = (i+errors.size()-1) % errors.size();
    else // right
      i = (i+1) % errors.size();
  }
  return 0;
} // end main
