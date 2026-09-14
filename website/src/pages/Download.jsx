import { FaLinux, FaWindows, FaApple } from "react-icons/fa"


export default function Home() {
  return (
    <main className="min-h-screen bg-zinc-950 text-white px-4 sm:px-8 py-16">
        <section className="max-w-4xl mx-auto text-center">
            <h1 className="text-4xl sm:text-5xl font-bold">
                Download Maryn
            </h1>
            <p className="mt-4 text-zinc-400">
                Choose your platform and start using Maryn Launcher .
            </p>
            <div className="mt-9 flex flex-col sm:flex-row justify-center gap-4">
                <a href="https://github.com/DinankaKoirala/Maryn_Launcher/releases/download/v0.2.0/Maryn-Launcher-Windows.zip" 
                className="flex items-center gap-2 text-gray-700 hover:text-blue-600">
                 <FaWindows className="w-5 h-5" />
                 Windows
                </a>

                <a href="https://github.com/DinankaKoirala/Maryn_Launcher/releases/download/v0.2.0/Maryn_Launcher-x86_64.AppImage" 
                className="flex items-center gap-2 text-gray-700 hover:text-white">
                 <FaLinux className="w-5 h-5" />
                 Linux
                </a>

                <a href="https://github.com/DinankaKoirala/Maryn_Launcher/releases/download/v0.2.0/Maryn-Launcher-Mac.dmg" 
                className="flex items-center gap-2 text-gray-700 hover:text-green-400">
                 <FaApple className="w-5 h-5" />
                 Mac
                </a>

            </div>
        </section>
    </main>
  )
}