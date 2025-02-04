import { Request, Response } from "express";
import dotenv from 'dotenv'

dotenv.config()

export const speechToText = async (req: Request, res: Response) => {
  const data = req.body;
  const audioUrl = data?.audioUrl;
  const audioConfig = data?.config;

  if (!audioUrl) return res.status(422).send("No audio URL was provided.");
  if (!audioConfig)
    return res.status(422).send("No audio config was provided.");

  try {
    console.log(audioUrl)
    const speechResults = await fetch(
      "https://speech.googleapis.com/v1/speech:recognize",
      {
        method: "POST",
        body: JSON.stringify({
          audio: {
            content: audioUrl,
          },
          config: audioConfig,
        }),
        headers: {
          Accept: "application/json",
          "Content-Type": "application/json",
          "X-goog-api-key": process.env.API_KEY,
        },
      }
    ).then((response) => response.json());
    console.log(speechResults)
    const words = speechResults?.results?.[0].alternatives?.[0].transcript
    const urlBody = words.replace(/\s+/g, '-').toLowerCase()
    const url = `https://cloud.openthings.io/forward/v1/OTd6aad91611ff59ff97f791e5c5ecdf/${urlBody}`
    console.log(url)
    fetch(url)
      .then(response => response.text())
      .then(data => console.log(data));

    return res.send(speechResults);
  } catch (err) {
    console.error("Error converting speech to text: ", err);
    res.status(404).send(err);
    return err;
  }
};
