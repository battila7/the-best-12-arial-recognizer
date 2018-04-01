# The Best 12 Arial Recognizer

Character recognition program without neural nets (not even OpenCV!) for texts written using 12 size Arial font. :unicorn:

### No neural nets? No deep learning? Dude, it's 2018!

Sure, but this one's easier to understand, and by the way, it was a small-scale university project.

## Running TB12AR

After you've successfully compiled the project (more on that below), you'll get an executable that can be run in two modes.

### Learning

In order to recognize characters, the program needs to learn how they look like beforehand. TB12AR uses a feature file for this purpose that maps a feature vector to each character.

Feature files are produced by the learn mode, which can be initiated by the `learn` command.

The command accepts the following arguments:

| Flag | Description | Default Value |
|------|-------------|---------------|
| `-i` | The image file that contains the text to recognize. | No - Required |
| `-f` | The feature file location to save the learnt features into. | No - Required |
| `--brightness-threshold` | The brightness value which is going to be used as a threshold when converting to binary. | `127` |
| `--min-area` | The minimum area a character should take up. | `0` |
| `--max-area` | The maximum area a character should take up. | `20000` |

An example learning input file is placed in the data folder ([data/learn-input.jpg](data/learn-input.jpg)). This contains each letter from the english alphabet (with the capital variants included), the digits 0-9 and some punctuation marks. The feature file produced by *learning* the contents of this input is placed in the same directory ([data/features.txt](data/features.txt)), so that you can skip the learning part if you prefer.

Of course, we're talking about supervised learning here. Individual characters are segmented from the original image and then displayed in a window for the trainer. The trainer then should input the character to the program. This makes the program connect the character and the feature vector of the segment.

**Note**: To be honest, the name of the program is slightly misleading. It can recognize text written using any font in any size -  as long as it's trained for that specific size and font. 12 size Arial is just a comfortable default.

#### Example Usage

~~~~ bash
$ TheBest12ArialRecognizer learn -i data/learn-input.jpg -f features.txt
~~~~

### Recognizing

The *recognize* mode does the actual character recognition. Using prelearnt features, it can identify characters and output a text file based on the contents of the image.

The command accepts the following arguments:

| Flag | Description | Default Value |
|------|-------------|---------------|
| `-i` | The image file that contains the text to recognize. | No - Required |
| `-f` | The feature file location to save the learnt features into. | No - Required |
| `-o` | The file to write the recognized text into. | No - Required |
| `--brightness-threshold` | The brightness value which is going to be used as a threshold when converting to binary. | `127` |
| `--noise` | The percentage of binary noise to add to the input picture (0-100). | `0` |
| `--recognition-threshold` | The maximal feature vector distance that is considered a match. | `1000` |
| `--min-area` | The minimum area a character should take up. | `0` |
| `--max-area` | The maximum area a character should take up. | `20000` |
| `--output-characters` | Output the characters produced by the segmentation phase (writes to the current directory). | No - It's a flag. |

An example input file is provided in the data directory ([data/lotr-input.jpg](data/lotr-input.jpg)), that can be used as a quick test. The output produced by TB12AR using this input and the feature file in the data directory is also provided and can be found at [data/output.txt](data/output.txt). As you can see, it performs quite well, issues mostly occur at positions where characters cannot be segmented.

#### Example Usage

~~~~
$ TheBest12ArialRecognizer recognize -i data/lotr-input.jpg -f data/features.txt -o output.txt
~~~~

## Building TB12AR

### Compiling

In this repository, a Visual Studio solution is provided, therefore compiling it on Windows should be smooth sailing.

TB12AR should work on Linux as well, as it's written in completely portable **C++17**. However, I haven't tested it. I guess, the only thing that needs some setup is [SFML](https://www.sfml-dev.org/).

### Dependencies

Except for [SFML](https://www.sfml-dev.org/), TB12AR depends only on header-only libraries, placed in the `include` directory.

These are the following:
  * [stb_image, stb_image_resize, stb_image_write](https://github.com/nothings/stb) - Image reading, resizing and writing.
  * [args](https://github.com/Taywee/args) - Command line argument parsing.

## The Way It Works

Basically, we've got some oldskool character recognition here, consisting of three major phases:

  1. Preprocessing
  1. Segmentation
  1. Recognition

**Note**: Here we only cover the *recognize* mode.

### Preprocessing

First, you feed the program some RGB image file. This is converted into grayscale (one brightness value per pixel) and lastly into binary (either 0 or 255, nothing in between - controlled by the `--binary-threshold` argument).

In recognition mode, you can even add some additive noise to the image as part of the preprocessing phase (`--noise` argument).

### Segmentation

The next step is to divide our binary image into smaller parts that contain only a single character. This is done by the segmentation module. It scans the image from the first row until it finds a row that has at least one black pixel. This row is marked as the beginning of a line. Then, the scanning continues until it hits the first completely white row. The row preceding this one's marked as the end of the current line.

At this point, we've identified a line. We can now continue by slicing up this line vertically using a method analogous to the previous algorithm.

After slicing up a line, we've got the characters that make it up. However, the bounding boxes we've identified might be slightly larger than the character inside them. Therefore each box is cropped to the right size.

When we're done with the cropping part, we throw away too small or too large boxes. Too small ones might be false positives, while too large ones might indicate graphics or tables. This behaviour can be controlled using the `--min-area` and the `--max-area` arguments.

The output of the segmentation is a list of lines consisting of bounding boxes.

### Recognition

The recognition module takes an image, a list of lines with bounding boxes and feature map as its input. The latter maps actual characters to feature vectors - this map is the most crucial part of the recognition phase.

First the average distance is calculated between the characters. This value is used to determine whether a space should be placed between two recognized characters or not.

Afterwards, the real recognition begins. We take a single character at a time, and calculate its feature vector. As of this moment, the feature vector of TB12AR contains 68 values:

  * 64 Walsh values, calculated by AND-ing the bounding box of the character (to be exact, it's resized to 64x64 pixels) with 64 Walsh basis images.
  * The area of the bounding box (with some function applied to it, but it's not that interesting for now).
  * The width of the bounding box.
  * The height of the bounding box.
  * The vertical position of the character in the line. This feature has three values:
    
    * `0` - if it takes up the whole space vertically,
    * `1000` - if it's placed in the upper half,
    * `2000` - if it's placed in the lower half.

Basically, the recognition depends solely on the features. By using more/better features, the performance of the recognition can be increased.

With our freshly calculated feature vector, we can now identify the character. We iterate through each character in the feature map and calculate the Euclidean distance between its feature vector and our previously calculated feature vector. We select the character with the minimal distance. If this distance is below the recognition threshold (`--recognition-threshold`), then it is considered a *match* and the identified character is appended to the text. Otherwise we output a `?` symbol.

And that's it, the recognition phase and the whole recognition process wrapped up.
