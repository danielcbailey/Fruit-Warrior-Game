package main

import (
	"bytes"
	"fmt"
	"image"
	"image/png"
	"io/ioutil"
	"path/filepath"
	"strconv"
	"strings"
)

func main() {
	fDir := "C:\\Users\\dcoop\\Documents\\Spring 2021\\2035Project2\\assets"
	fName := "background.png"

	frameWidth := 128
	frameHeight := 128

	assetNames := []string{"backgroundGame"}
	groupName := "backgrounds"

	useLUT := false

	b, e := ioutil.ReadFile(filepath.Join(fDir, fName))

	if e != nil {
		panic(e)
	}

	img, e := png.Decode(bytes.NewBuffer(b))

	if e != nil {
		panic(e)
	}

	allLines := []string{"#include <stdint.h>", "#include \"graphics/bitmapImage.hpp\"", "#include \"graphics/colorLUT.hpp\"",
						 "//This file is autogenerated and contains asset data to be stored in ROM.",
						 "//(c) Daniel Cooper and Artists 2021"}

	var lut []uint16
	var lines []string
	lutName := "nullptr"
	if useLUT {
		lutName = "&" + groupName + "_colorLUT"
	}

	for aI, aName := range assetNames {
		for i := 0; img.Bounds().Dx() / frameWidth > i; i++ {
			lines, lut = createAsset(i * frameWidth, aI * frameHeight, frameWidth, frameHeight, lines, aName + "_" + strconv.Itoa(i),
						img, lut, lutName)
		}
	}

	if useLUT {
		//generating the LUT
		allLines = append(allLines, "", "const uint16_t " + lutName[1:] + "_lut[] = { 0,")

		for _, v := range lut {
			if len(allLines[len(allLines) - 1]) >= 74 {
				allLines = append(allLines, "\t\t")
			}

			allLines[len(allLines) - 1] += " " + strconv.Itoa(int(v)) + ","
		}

		allLines[len(allLines) - 1] = allLines[len(allLines) - 1][:len(allLines[len(allLines) - 1]) - 1] + "};"
		allLines = append(allLines, "", "ColorLUT " + lutName[1:] + " = ColorLUT((uint16_t*)" + lutName[1:] + "_lut);")
	}

	allLines = append(allLines, lines...)

	//writing the file
	builder := strings.Builder{}
	for i, l := range allLines {
		if i != 0 {
			builder.WriteByte('\n')
		}

		builder.WriteString(l)
	}

	outPath := filepath.Join(fDir, groupName + ".cpp")
	e = ioutil.WriteFile(outPath, []byte(builder.String()), 0644)

	if e != nil {
		panic(e)
	}

	fmt.Println("Saved the image data.")
}

func createAsset(xs, ys, width, height int, lines []string, name string, image image.Image, lut []uint16, lutName string) ([]string, []uint16) {
	//creating the bitmap LUT
	if lutName != "nullptr" {
		lines = append(lines, "", "const uint8_t " + name + "_bitmap[] = {")
	} else {
		lines = append(lines, "", "const uint16_t " + name + "_bitmap[] = {")
	}

	for y := ys; height + ys > y; y++ {
		for x := xs; width + xs > x; x++ {
			//sampling pixel and converting it to 4DGL format
			pixel := image.At(x, y);

			r, g, b, a := pixel.RGBA()

			if a < 0xFF00 && lutName != "nullptr" {
				//will not accept the pixel and will handle as if transparent

				if len(lines[len(lines) - 1]) >= 74 {
					lines = append(lines, "\t\t")
				}

				lines[len(lines) - 1] += " 0,"
				continue
			}

			g >>= 10
			r >>= 11
			b >>= 11

			var value uint16
			red5   := r & 0x1F                  // get red on 5 bits
			green6 := g & 0x3F                  // get green on 6 bits
			blue5  := b & 0x1F                  // get blue on 5 bits
			value = uint16(((((green6 << 5) + (blue5 >> 0)) & 0xFF) << 8) | (((red5 << 3)   + (green6 >> 3)) & 0xFF))
			//value = uint16((((g >> 3) & 0x7) << 13) | ((b & 0x1F) << 8) | ((r & 0x1F) << 3) | (g & 0x7))

			if lutName != "nullptr" {
				//look for color in lut, if not there, then add it
				//only works for up to 255 colors
				found := false
				for i, v := range lut {
					if v == value {
						value = uint16(i + 1)
						found = true
						break
					}
				}

				if !found {

					lut = append(lut, value)
					value = uint16(len(lut))

					if len(lut) >= 256 {
						panic("Color amount assumption failed")
					}
				}
			}

			if len(lines[len(lines) - 1]) >= 74 {
				lines = append(lines, "\t\t")
			}

			lines[len(lines) - 1] += " " + strconv.Itoa(int(value)) + ","
		}
	}

	//the last line will have one extra comma, time to remove it and add closing bracket
	lines[len(lines) - 1] = lines[len(lines) - 1][:len(lines[len(lines) - 1]) - 1] + "};"

	//adding the instantiation of the bitmap object
	lines = append(lines, "", "BitmapImage " + name +
		" = BitmapImage((const uint8_t*)" + name + "_bitmap, " + strconv.Itoa(width) + ", " +
		strconv.Itoa(height) + ", " + lutName + ");")

	return lines, lut
}