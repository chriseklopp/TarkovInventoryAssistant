using Interop;
using Microsoft.AspNetCore.Mvc;
using SkiaSharp;
using System.Diagnostics;
using TarkovInventoryAssistant_Server.Models;

namespace TarkovInventoryAssistant_Server.Controllers
{
    public class TIAController : Controller
    {



        public TIAController(CoreInterop core)
        {
            m_core = core;
        }

        // Show the selected image from the active catalog.
        public IActionResult ShowCatalogImage(string filePath, string fileName, string contentType)
        {
            var fileContentResult = new FileContentResult(System.IO.File.ReadAllBytes(filePath), contentType)
            {
                FileDownloadName = fileName
            };

            return fileContentResult;
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Privacy()
        {
            return View();
        }


        [HttpPost]
        public IActionResult AddImage(IFormFile image)
        {
            if (image != null && image.Length > 0)
            {
                using (var memoryStream = new MemoryStream())
                {
                    image.CopyTo(memoryStream);
                    byte[] bytes = memoryStream.ToArray();
                    return new FileContentResult(bytes, "image/png"); // Change "image/jpeg" to the appropriate content type for your image
                }
            }
            return Content("<p>No image was uploaded.</p>");
        }

        [HttpPost]
        public IActionResult DisplayResults(IFormFile imageFile)
        {

            if (imageFile == null || imageFile.Length == 0)
            {
                return BadRequest("Invalid Image file. ");
            }

            SKBitmap image;
            using (var stream = imageFile.OpenReadStream())
            {
                image = SKBitmap.Decode(stream);
            }

            DetectionResultMarshal[] detMat = m_core.detectImageContent(image);

            bool collapseSimilar = true; // TODO: this needs to be an option in the View.

            List<DetectionResultsModel> results = new List<DetectionResultsModel>(detMat.Length);
            string catalogPath = m_core.getACTIVE_CATALOG();

            if (collapseSimilar)
            {
                // name, <detMatIndex, count> detMatIndex is index of the first item of the given name.
                Dictionary<string, Utilities.Pair<int, int>> uniqueDetectionMap = new Dictionary<string, Utilities.Pair<int, int>>();

                for (int i = 0; i < detMat.Length; i++)
                {
                    DetectionResultMarshal drm = detMat[i];
                    if (uniqueDetectionMap.ContainsKey(drm.name))
                    {
                        uniqueDetectionMap[drm.name].Second++;
                    }
                    else
                    {
                        uniqueDetectionMap[drm.name] = new Utilities.Pair<int, int>(i, 1);
                    }
                }
                foreach (string name in uniqueDetectionMap.Keys)
                {
                    results.Add(new DetectionResultsModel(detMat[uniqueDetectionMap[name].First], catalogPath, uniqueDetectionMap[name].Second));
                }

            }
            else
            {
                // Simply add every DetectionResult
                for (int i = 0; i < detMat.Length; i++)
                {
                    results.Add(new DetectionResultsModel(detMat[i], catalogPath));
                }
            }

            return PartialView("Result", results);
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }


        private readonly CoreInterop m_core;

    }
}