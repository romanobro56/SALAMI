import { Request, Response } from "express";

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
          "X-goog-api-key": `AIzaSyAeMlzxoNEEdyyy5gjCduOcF_n7IzyhUo0`,
        },
      }
    ).then((response) => response.json());
    console.log(speechResults)
    return res.send(speechResults);
  } catch (err) {
    console.error("Error converting speech to text: ", err);
    res.status(404).send(err);
    return err;
  }
};
