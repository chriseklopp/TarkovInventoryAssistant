using Interop;
using Microsoft.AspNetCore.Mvc;
using System.Diagnostics;
using TarkovInventoryAssistant_Server.Models;
using SkiaSharp;
using static System.Net.Mime.MediaTypeNames;

namespace TarkovInventoryAssistant_Server.Controllers
{
    public class HomeController : Controller
    {
        private readonly CoreInterop m_core;

        public HomeController(CoreInterop core)
        {
            m_core = core;
        }

        public IActionResult Index()
        {
            return View();
        }

        public IActionResult Privacy()
        {
            return View();
        }

        public IActionResult Result(List<DetectionResultsModel> detections)
        {
            return View(detections);
        }


        public IActionResult ShowImage(string filePath, string fileName, string contentType)
        {
            var fileContentResult = new FileContentResult(System.IO.File.ReadAllBytes(filePath), contentType)
            {
                FileDownloadName = fileName
            };

            return fileContentResult;
        }

        [HttpPost]
        public IActionResult SubmitImage()
        {

            var imageFile = HttpContext.Request.Form.Files.GetFile("image");
            if (imageFile == null || imageFile.Length == 0)
            {
                return BadRequest("Please select an image file.");
            }

            SKBitmap image;
            using (var stream = imageFile.OpenReadStream())
            {
                image = SKBitmap.Decode(stream);
            }


            DetectionResultMarshal[] detMat = m_core.detectImageContent(image);
            List<DetectionResultsModel> results = new List<DetectionResultsModel>(detMat.Length);
            string catalogPath = m_core.getACTIVE_CATALOG();
            for (int i = 0; i < detMat.Length; i++)
            {
                results.Add(new DetectionResultsModel(detMat[i], catalogPath));
            }
            return View("Result", results);
        }

        [ResponseCache(Duration = 0, Location = ResponseCacheLocation.None, NoStore = true)]
        public IActionResult Error()
        {
            return View(new ErrorViewModel { RequestId = Activity.Current?.Id ?? HttpContext.TraceIdentifier });
        }



    }
}